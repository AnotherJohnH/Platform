//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#pragma once

#include <cstring>

#include "STB/FileSystem.h"

namespace STB {

class FAT16 : public FileSystem
{
public:
   FAT16(const char* label_,
         unsigned    size_mbytes_)
      : vbr_head(label_, size_mbytes_)
   {
      fat[0] = 0xFFF8;
      fat[1] = 0xFFFF;

      root_dir[0].setVolumeLabel(label_);
   }

   void addFile(const char* filename_, uint32_t size_)
   {
      unsigned cluster = root_dir_entries + 2;

      fat[cluster] = 0xFFFF;

      root_dir[++root_dir_entries].setFile(filename_, cluster, size_);
   }

   //! Get pointer to raw file system data at the given address and segment index
   const uint8_t* get64BytePtr(uint32_t sector_, unsigned index_) const override
   {
      if (sector_ == 0)
      {
              if (index_ == 0) return vbr_head.getPtr();
         else if (index_ == 7) return vbr_tail.getPtr();
         else                  return empty;
      }
      else if ((sector_ == 1) ||
               (sector_ == (1 + vbr_head.sectors_per_fat)))
      {
         return index_ == 0 ? (const uint8_t*)fat : empty;
      }
      else if (sector_ == (1 + 2 * vbr_head.sectors_per_fat))
      {
         return ((const uint8_t*)root_dir) + index_ * 64;
      }

      return empty;
   }

   unsigned getBlockSize() const override { return VBRHead::BYTES_PER_SECTOR; }

   unsigned getNumBlocks() const override { return vbr_head.getNumSectors(); }

private:

   class VBRHead
   {
   public:
      VBRHead(const char* label_,
              unsigned    size_mbytes_)
      {
         unsigned size = size_mbytes_ * 1024 * 1024;

         num_sectors = size / BYTES_PER_SECTOR;

         if (num_sectors < 0x10000)
         {
            total_sectors_16 = num_sectors - 1;
            total_sectors_32 = 0;
         }
         else
         {
            total_sectors_16 = 0;
            total_sectors_32 = num_sectors - 1;
         }

         cluster_count          = size / BYTES_PER_CLUSTER;
         unsigned bytes_per_fat = cluster_count * sizeof(uint16_t);
         sectors_per_fat        = 1 + ((bytes_per_fat + BYTES_PER_SECTOR - 1) / BYTES_PER_SECTOR);

         for(unsigned i = 0; i < 11; ++i)
         {
            if (*label_ == '\0')
               volume_label[i] = ' ';
            else
               volume_label[i] = *label_++;
         }
      }

      unsigned getNumSectors() const { return num_sectors; }

      const uint8_t* getPtr() const { return jump_inst; }

      static const unsigned MAX_ROOT_DIR_ENTRIES = 512;
      static const unsigned BYTES_PER_SECTOR     = 512;
      static const unsigned SECTORS_PER_CLUSTER  = 8;
      static const unsigned BYTES_PER_CLUSTER    = SECTORS_PER_CLUSTER * BYTES_PER_SECTOR;
      static const unsigned VBR_SIZE             = BYTES_PER_SECTOR;

   // private:
      uint8_t  jump_inst[3] = {0xEB, 0x3C, 0x90};  // x86: jmp 0x3e; nop
      uint8_t  oem_name[8]  = {'P', 'l', 'a', 't', 'f', 'o', 'r', 'm'};
      uint16_t bytes_per_sector{BYTES_PER_SECTOR};
      uint8_t  sectors_per_cluster{SECTORS_PER_CLUSTER};
      uint16_t reserved_sectors{1};
      uint8_t  number_of_fats{2};
      uint16_t max_root_dir_entries{MAX_ROOT_DIR_ENTRIES};
      uint16_t total_sectors_16;
      uint8_t  media_descr{0xF8};
      uint16_t sectors_per_fat;
      uint16_t sectors_per_track{0x01};
      uint16_t number_of_heads{0x01};
      uint32_t hidden_sectors{0x01};
      uint32_t total_sectors_32;
      uint8_t  drive_number{0x00};
      uint8_t  reserved{0x00};
      uint8_t  boot_signature{0x29};
      uint32_t volume_id{0x00000000};
      uint8_t  volume_label[11];
      uint8_t  file_system_type[8] = {'F', 'A', 'T', '1', '6', ' ', ' ', ' '};
      uint8_t  boot_strap_code[2]  = {0xEB, 0xFE}; // x86: jmp self

      unsigned num_sectors{};
      unsigned cluster_count{};

   } __attribute__((__packed__));

   class VBRTail
   {
   public:
      VBRTail()
      {
         byte[62] = 0x55;
         byte[63] = 0xAA;
      }

      const uint8_t* getPtr() const { return byte; }

   private:
      uint8_t byte[64] = {};
   };

   struct DirEntry
   {
      DirEntry() = default;

      void setVolumeLabel(const char* name_)
      {
         for(unsigned i = 0; i < sizeof(name); ++i)
            if (*name_ == '\0')
               name[i] = ' ';
            else
               name[i] = *name_++;

         attr = ATTR_ARCHIVE | ATTR_VOLUME_LABEL;
      }

      void setFile(const char* name_, uint16_t cluster_, uint32_t size_)
      {
         unsigned i;

         for(i = 0; i < 8; ++i)
            if ((*name_ == '\0') || (*name_ == '.'))
               name[i] = ' ';
            else
               name[i] = *name_++;

         if (*name_ == '.')
            name_++;

         for(; i < sizeof(name); ++i)
            if (*name_ == '\0')
               name[i] = ' ';
            else
               name[i] = *name_++;

         attr     = ATTR_ARCHIVE | ATTR_READ_ONLY;
         cluster  = cluster_;
         size     = size_;
      }

      void clear()
      {
         memset(this, 0, sizeof(DirEntry));
      }

      static const uint8_t ATTR_NONE         = 0x00;
      static const uint8_t ATTR_READ_ONLY    = 0x01;
      static const uint8_t ATTR_HIDDEN       = 0x02;
      static const uint8_t ATTR_SYSTEM       = 0x04;
      static const uint8_t ATTR_VOLUME_LABEL = 0x08;
      static const uint8_t ATTR_DIRECTORY    = 0x10;
      static const uint8_t ATTR_ARCHIVE      = 0x20;

      uint8_t  name[11] = {0};
      uint8_t  attr{0};
      uint8_t  reserved{0};
      uint8_t  create_time_tenths{0};
      uint16_t create_time{0};
      uint16_t create_date{0};
      uint16_t access_date{0};
      uint16_t cluster_hi{0};          //!< FAT32
      uint16_t mod_time{0};
      uint16_t mod_date{0};
      uint16_t cluster{0};
      uint32_t size{0};

   } __attribute__((__packed__));

   VBRHead  vbr_head;
   VBRTail  vbr_tail{};
   uint16_t fat[32] = {};
   unsigned root_dir_entries{0};
   DirEntry root_dir[16] = {};
   uint8_t  empty[64] = {};
};

template <unsigned NUM_CLUSTERS>
struct Table
{
   uint16_t entries[NUM_CLUSTERS];
};

} // namespace VBR
