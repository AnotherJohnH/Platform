//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
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
