//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// See RFC-1950

#include <cstdint>

#include "STB/Zlib.h"

namespace STB {

size_t ZLib::inflate()
{
   static const unsigned ZLIB_CM_DEFLATE = 8;

   uint8_t cmf = getByte();
   uint8_t flg = getByte();

   unsigned compression_method = cmf & 0b1111;
   unsigned compression_info   = (cmf >> 4) & 0b1111;
   bool     preset_dictionary  = (flg & 0b100000) != 0;

   if (preset_dictionary)
   {
      error("ZLIB stream with preset dictionary not supported");
      return 0;
   }

   size_t size = 0;

   adler32reset();

   switch(compression_method)
   {
   case ZLIB_CM_DEFLATE:
      {
         Deflate  deflate{this};
         unsigned log2_window_size = 8 + compression_info;

         size = deflate.inflate(log2_window_size);
      }
      break;

   default:
      error("compression method must be DEFLATE");
      break;
   }

   if (size != 0)
   {
      uint32_t adler32;
      adler32  = getByte() << 24;
      adler32 |= getByte() << 16;
      adler32 |= getByte() << 8;
      adler32 |= getByte();

      if (adler32 != adler32crc())
      {
         error("CRC failure");
      }
    }

   return size;
}

}
