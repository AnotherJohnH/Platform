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

#ifndef STB_ZLIB_H
#define STB_ZLIB_H

#include <cstdint>

#include "Deflate.h"

namespace STB {

class ZLib : public Deflate::Io
{
public:
   using Io = Deflate::Io;
   
   ZLib(Io* io_) : io(io_) {}
   
   //! Inflate Z-lib I/O stream
   size_t inflate(); 

   //! Update CRC with the Adler-32 algorithm
   static void adler32CRC(uint32_t& crc, uint8_t byte)
   {
      static const uint32_t BASE = 65521; // Largest prime smaller that 65536

      uint32_t s1 = crc & 0xFFFF;
      uint32_t s2 = crc >> 16;

      s1 = (s1 + byte) % BASE;
      s2 = (s1 + s2) % BASE;

      crc = (s2 << 16) | s1;
   }

private:
   virtual uint8_t getByte()
   {
      return io->getByte();
   }

   virtual bool putByte(uint8_t byte)
   {
      adler32CRC(crc, byte);
      return io->putByte(byte);
   }

   Io*      io{nullptr};
   uint32_t crc{0};
};

} // namespace STB

#endif
