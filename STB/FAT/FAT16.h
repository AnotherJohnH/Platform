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
#include "STB/FAT/Table.h"
#include "STB/FAT/Dir.h"

namespace STB {

template <unsigned SIZE_MB,
          uint32_t BYTES_PER_SECTOR     = 512,
          uint32_t MAX_ROOT_DIR_ENTRIES = 16,    //!< One sector
          unsigned SECTORS_PER_CLUSTER  = 8>     //!< 4 KiB
class FAT16 : public FileSystem
{
public:
   FAT16(const char* label_)
      : vbr(label_)
      , root_dir(label_)
   {
   }

   void addFile(const char* filename_, uint32_t size_, uint8_t* raw_data_)
   {
      uint16_t cluster = fat.alloc(bytesToClusters(size_));
      signed   index   = root_dir.addFile(filename_, cluster, size_);

      if (index >= 0)
      {
         dir_entry_data[index] = raw_data_;
      }
   }

private:
   uint8_t* getFilePointer(uint32_t sector_, unsigned offset_) const
   {
      uint32_t cluster = 2 + (sector_ - LBA_DATA) / SECTORS_PER_CLUSTER;
      unsigned dir_index;
      unsigned cluster_seq;

      if (root_dir.findFile(cluster, fat, dir_index, cluster_seq))
      {
         if (dir_entry_data[dir_index] != nullptr)
         {
            uint32_t cluster_offset = ((sector_ - LBA_DATA) % SECTORS_PER_CLUSTER) + offset_;
            uint32_t data_offset    = cluster_offset + cluster_seq * BYTES_PER_CLUSTER;

            return dir_entry_data[dir_index] + data_offset;
         }
      }

      return nullptr;
   }

   unsigned getBlockSize() const override { return BYTES_PER_SECTOR; }

   unsigned getNumBlocks() const override { return NUM_SECTORS; }

   void read(uint32_t sector_,
             unsigned offset_,
             unsigned bytes_,
             uint8_t* buffer_) const override
   {
      if (sector_ == LBA_VBR)
      {
         vbr.read(offset_, bytes_, buffer_);
      }
      else if ((sector_ >= LBA_FAT1) && (sector_ < LBA_FAT2))
      {
         unsigned fat_offset = (sector_ - LBA_FAT1) * BYTES_PER_SECTOR + offset_;

         fat.read(fat_offset, bytes_, buffer_);
      }
      else if ((sector_ >= LBA_FAT2) && (sector_ < LBA_ROOT_DIR))
      {
         unsigned fat_offset = (sector_ - LBA_FAT2) * BYTES_PER_SECTOR + offset_;

         fat.read(fat_offset, bytes_, buffer_);
      }
      else if ((sector_ >= LBA_ROOT_DIR) && (sector_ < LBA_DATA))
      {
         unsigned dir_offset = (sector_ - LBA_ROOT_DIR) * BYTES_PER_SECTOR + offset_;

         root_dir.read(dir_offset, bytes_, buffer_);
      }
      else
      {
         uint8_t* ptr = getFilePointer(sector_, offset_);
         if (ptr != nullptr)
         {
            ::memcpy(buffer_, ptr, bytes_);
         }
         else
         {
            ::memset(buffer_, 0, bytes_);
         }
      }
   }

   void write(uint32_t       sector_,
              unsigned       offset_,
              unsigned       bytes_,
              const uint8_t* buffer_) override
   {
      if (sector_ == LBA_VBR)
      {
         // XXX ignore writes to the VBR
      }
      else if ((sector_ >= LBA_FAT1) && (sector_ < LBA_FAT2))
      {
         unsigned fat_offset = (sector_ - LBA_FAT1) * BYTES_PER_SECTOR + offset_;

         fat.write(fat_offset, bytes_, buffer_);
      }
      else if ((sector_ >= LBA_FAT2) && (sector_ < LBA_ROOT_DIR))
      {
         unsigned fat_offset = (sector_ - LBA_FAT2) * BYTES_PER_SECTOR + offset_;

         fat.write(fat_offset, bytes_, buffer_);
      }
      else if ((sector_ >= LBA_ROOT_DIR) && (sector_ < LBA_DATA))
      {
         unsigned dir_offset = (sector_ - LBA_ROOT_DIR) * BYTES_PER_SECTOR + offset_;

         root_dir.write(dir_offset, bytes_, buffer_);
      }
      else
      {
      }
   }

   void endOfWrite() override
   {
   }

   class VBR
   {
   public:
      VBR(const char* label_)
      {
         if (TOTAL_SECTORS < 0x10000)
         {
            total_sectors_16 = TOTAL_SECTORS;
         }
         else
         {
            total_sectors_32 = TOTAL_SECTORS;
         }

         for(unsigned i = 0; i < 11; ++i)
         {
            if (*label_ == '\0')
               volume_label[i] = ' ';
            else
               volume_label[i] = *label_++;
         }
      }

      void read(unsigned offset_,
                unsigned bytes_,
                uint8_t* buffer_) const
      {
         // XXX Assumes bytes_ is 64 and offset_ is 64 byte aligned

         if (offset_ == 0)
         {
            ::memcpy(buffer_, jump_inst, bytes_);
         }
         else if (offset_ == 0x1C0)
         {
            ::memset(buffer_, 0, 62);
            buffer_[62] = 0x55;
            buffer_[63] = 0xAA;
         }
         else
         {
            ::memset(buffer_, 0, bytes_);
         }
      }

   private:
      uint8_t  jump_inst[3] = {0xEB, 0x3C, 0x90};  // x86: jmp 0x3e; nop
      uint8_t  oem_name[8]  = {'P', 'l', 'a', 't', 'f', 'o', 'r', 'm'};
      uint16_t bytes_per_sector{BYTES_PER_SECTOR};
      uint8_t  sectors_per_cluster{SECTORS_PER_CLUSTER};
      uint16_t reserved_sectors{1};
      uint8_t  number_of_fats{2};
      uint16_t max_root_dir_entries{MAX_ROOT_DIR_ENTRIES};
      uint16_t total_sectors_16{0};
      uint8_t  media_descr{0xF8};
      uint16_t sectors_per_fat{SECTORS_PER_FAT};
      uint16_t sectors_per_track{0x01};
      uint16_t number_of_heads{0x01};
      uint32_t hidden_sectors{0x01};
      uint32_t total_sectors_32{0};
      uint8_t  drive_number{0x00};
      uint8_t  reserved{0x00};
      uint8_t  boot_signature{0x29};
      uint32_t volume_id{0x00000000};
      uint8_t  volume_label[11];
      uint8_t  file_system_type[8] = {'F', 'A', 'T', '1', '6', ' ', ' ', ' '};
      uint8_t  boot_strap_code[2]  = {0xEB, 0xFE}; // x86: jmp self

   } __attribute__((__packed__));

   //! Convert a size in bytes to a whole number of sectors
   static constexpr unsigned bytesToSectors(unsigned bytes_)
   {
      return (bytes_ + BYTES_PER_SECTOR - 1) / BYTES_PER_SECTOR;
   }

   //! Convert a size in bytes to a whole number of clusters
   static constexpr unsigned bytesToClusters(unsigned bytes_)
   {
      return (bytes_ + BYTES_PER_CLUSTER - 1) / BYTES_PER_CLUSTER;
   }

   static constexpr unsigned NUM_SECTORS       = SIZE_MB * 1024 * 1024 / BYTES_PER_SECTOR;
   static constexpr unsigned TOTAL_SECTORS     = NUM_SECTORS - 1;
   static constexpr unsigned NUM_CLUSTERS      = NUM_SECTORS / SECTORS_PER_CLUSTER;
   static constexpr unsigned BYTES_PER_FAT     = NUM_CLUSTERS * sizeof(uint16_t);
   static constexpr unsigned SECTORS_PER_FAT   = 1 + bytesToSectors(BYTES_PER_FAT);
   static constexpr unsigned SECTORS_IN_ROOT   = bytesToSectors(MAX_ROOT_DIR_ENTRIES * sizeof(FAT::DirEntry));
   static constexpr unsigned BYTES_PER_CLUSTER = BYTES_PER_SECTOR * SECTORS_PER_CLUSTER;

   static constexpr uint32_t LBA_VBR      = 0;
   static constexpr uint32_t LBA_FAT1     = LBA_VBR + 1;
   static constexpr uint32_t LBA_FAT2     = LBA_FAT1 + SECTORS_PER_FAT;
   static constexpr uint32_t LBA_ROOT_DIR = LBA_FAT2 + SECTORS_PER_FAT;
   static constexpr uint32_t LBA_DATA     = LBA_ROOT_DIR + SECTORS_IN_ROOT;

   VBR                            vbr;
   FAT::Table16<NUM_CLUSTERS>     fat{};
   FAT::Dir<MAX_ROOT_DIR_ENTRIES> root_dir;
   uint8_t*                       dir_entry_data[MAX_ROOT_DIR_ENTRIES] = {};
};

} // namespace VBR
