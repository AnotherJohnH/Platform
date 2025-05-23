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

namespace STB {

namespace FAT {

class DirEntry
{
public:
   DirEntry() = default;

   bool isEmpty() const { return cluster == 0; }

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
   uint16_t cluster{0};
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

   void read(unsigned offset_, unsigned bytes_, uint8_t* buffer_) const
   {
      ::memcpy(buffer_, (const uint8_t*)entry + offset_, bytes_);
   }

   void write(unsigned offset_, unsigned bytes_, const uint8_t* buffer_) const
   {
      ::memcpy((uint8_t*)entry + offset_, buffer_, bytes_);
   }

   void addFile(const char* filename_, uint16_t cluster_, uint32_t size_)
   {
      signed index = alloc();
      if (index != -1)
      {
         entry[index].setFile(filename_, cluster_, size_);
      }
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
