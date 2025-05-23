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
#include "STB/FAT/DirEntry.h"

namespace STB {

template <unsigned SIZE_MB>
class FAT16 : public FileSystem
{
public:
   FAT16(const char* label_)
      : vbr_head(label_, SIZE_MB)
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

private:
   unsigned getBlockSize() const override { return VBRHead::BYTES_PER_SECTOR; }

   unsigned getNumBlocks() const override { return vbr_head.getNumSectors(); }

   void read(uint32_t sector_,
             unsigned offset_,
             unsigned bytes_,
             uint8_t* buffer_) const override
   {
      for(unsigned i = 0; i < bytes_; i += 64)
      {
         const uint8_t* ptr = empty;

         if (sector_ == 0)
         {
                 if (offset_ == 0)     ptr = vbr_head.getPtr();
            else if (offset_ == 0x1C0) ptr = vbr_tail.getPtr();
         }
         else if ((sector_ == 1) ||
                  (sector_ == (1 + vbr_head.sectors_per_fat)))
         {
            if (offset_ == 0) ptr = (const uint8_t*)fat;
         }
         else if (sector_ == (1 + 2 * vbr_head.sectors_per_fat))
         {
            ptr = ((const uint8_t*)root_dir) + offset_;
         }

         ::memcpy(buffer_, ptr, bytes_);

         buffer_ += 64;
         offset_ += 64;
      }
   }

   void write(uint32_t       sector_,
              unsigned       offset_,
              unsigned       bytes_,
              const uint8_t* buffer_) override
   {
   }

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

   VBRHead       vbr_head;
   VBRTail       vbr_tail{};
   uint16_t      fat[32] = {};
   unsigned      root_dir_entries{0};
   FAT::DirEntry root_dir[16] = {};
   uint8_t       empty[64] = {};
};

} // namespace VBR
