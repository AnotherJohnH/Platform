//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstring>

namespace STB {

namespace FAT {

class DirEntry
{
public:
   DirEntry() = default;

   bool isEmpty() const { return (cluster_hi == 0) && (cluster_lo == 0); }

   uint32_t getCluster() const { return (cluster_hi << 16) | cluster_lo; }

   uint32_t getSize() const { return size; }

   void setVolumeLabel(const char* name_)
   {
      for(unsigned i = 0; i < sizeof(name); ++i)
         if (*name_ == '\0')
            name[i] = ' ';
         else
            name[i] = *name_++;

      attr = ATTR_ARCHIVE | ATTR_VOLUME_LABEL;
   }

   void setFile(const char* name_, uint32_t cluster_, uint32_t size_)
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

      attr       = ATTR_ARCHIVE | ATTR_READ_ONLY;
      cluster_lo = cluster_ & 0xFFFF;
      cluster_hi = cluster_ >> 16;
      size       = size_;

      reserved           = 0;
      create_time_tenths = 0;
      create_time        = 0;
      create_date        = 0;
      access_date        = create_date;
      mod_time           = create_time;
      mod_date           = create_date;
   }

   void clear()
   {
      ::memset(this, 0, sizeof(DirEntry));
   }

private:
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
   uint16_t cluster_lo{0};
   uint32_t size{0};

} __attribute__((__packed__));


template <unsigned NUM_ENTRIES>
class Dir
{
public:
   Dir(const char* volume_label_ = nullptr)
   {
      if (volume_label_ != nullptr)
         entry[0].setVolumeLabel(volume_label_);
   }

   const DirEntry& operator[](unsigned index_) { return entry[index_]; }

   void read(unsigned offset_, unsigned bytes_, uint8_t* buffer_) const
   {
      ::memcpy(buffer_, (const uint8_t*)entry + offset_, bytes_);
   }

   void write(unsigned offset_, unsigned bytes_, const uint8_t* buffer_) const
   {
      ::memcpy((uint8_t*)entry + offset_, buffer_, bytes_);
   }

   signed addFile(const char* filename_, uint32_t cluster_, uint32_t size_)
   {
      signed index = alloc();
      if (index >= 0)
      {
         entry[index].setFile(filename_, cluster_, size_);
      }
      return index;
   }

   //! Find a directory entry that contains the given cluster
   template <typename FAT>
   bool findFile(uint32_t   search_cluster_,
                 const FAT& fat_,
                 unsigned&  dir_index_,
                 unsigned&  cluster_seq_) const
   {
      for(unsigned i = 0; i < NUM_ENTRIES; ++i)
      {
         if (not entry[i].isEmpty())
         {
            uint32_t cluster = entry[i].getCluster();

            for(unsigned j = 0; cluster < 0xFFF8; j++)
            {
               if (cluster == search_cluster_)
               {
                  dir_index_   = i;
                  cluster_seq_ = j;
                  return true;
               }

               cluster = fat_[cluster];
            }
         }
      }

      return false;
   }

private:
   //! Find an empty entry
   signed alloc() const
   {
      for(unsigned i = 0; i < NUM_ENTRIES; ++i)
      {
         if (entry[i].isEmpty())
            return i;
      }

      return -1;
   }

   DirEntry entry[NUM_ENTRIES];
};

} // namespace FAT

} // namespace STB
