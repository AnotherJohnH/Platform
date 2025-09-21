//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <cstring>

namespace STB {

class ArCxx
{
public:
   static const uint8_t* find(const char* filename, unsigned& size)
   {
      for(unsigned i = 0; i < num_entries; ++i)
      {
         if (::strcmp(filename, entry[i].filename) == 0)
         {
            size = entry[i].size;
            return entry[i].data;
         }
      }

      size = 0;
      return nullptr;
   }

private:
   struct Entry
   {
      const char*    filename;
      const uint8_t* data;
      unsigned       size;
   };

   static const unsigned num_entries;
   static const Entry    entry[];
};

} // namespace STB

