//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

//! \file  PixelGen.h
//! \brief Pixel stream generator for micro::bit V2

#ifndef MTL_MICROBIT_V2_PIXEL_GEN_H
#define MTL_MICROBIT_V2_PIXEL_GEN_H

#include  <cstring>

#include "MTL/Digital.h"
#include "MTL/nRF52/I2S.h"

namespace MTL {

template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned SCAN_REPEAT = 1,
          unsigned PIN_LUM     = PIN_PAD_16>
class PixelGen
{
private:
   static const unsigned PIXEL_FREQ = WIDTH >= 256 ? 8000000 :
                                      WIDTH >= 128 ? 4000000 :
                                                     2000000;

   // Pixel data from I2S_SDOUT
   MTL::I2S<PIN_LUM>          i2s{PIXEL_FREQ};
   MTL::Digital::Out<PIN_LUM> lum;

   const uint8_t*          frame;
   const uint8_t*          start;
   unsigned                width;
   uint8_t                 bytes_per_line;
   uint32_t                size;
   volatile const uint8_t* next;
   volatile uint8_t        row;

public:
   PixelGen()
   {
      setWidth(WIDTH);
   }

   //! Set pointer to frame buffer
   void setFramePtr(const uint8_t* ptr)
   {
      frame = start = ptr;
   }

   //! Set offset for top-left pixel from start of frame buffer (bytes)
   void setOffset(uintptr_t offset)
   {
      start = frame + offset;
   }

   //! Set frame width (pixels)
   //  Must be less than or equal to WIDTH
   //  Must be a multiple of 8
   void setWidth(unsigned width_)
   {
      width          = width_;
      bytes_per_line = width/8;
      size           = bytes_per_line * HEIGHT;
   }

   //! Get ready for next field of image
   void startField()
   {
      row  = 0;
      next = start;
      i2s.setTxLength(width/32);
   }

   //! Start generating pixel data
   void startLine()
   {
      i2s.start();
      if (++row == SCAN_REPEAT)
      {
         row = 0;
         next += bytes_per_line;
         if (next >= (frame + size))
         {
            next -= size;
         }
      }
   }

   //! Stop generating pixel data
   void endLine()
   {
      i2s.stop();
      i2s.setTxData((const uint32_t*)next);
   }
};

} // namespace MTL

#endif // MTL_MICROBIT_V2_PIXELGEN_H
