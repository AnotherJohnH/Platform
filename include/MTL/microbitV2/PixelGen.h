//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
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
#include "MTL/nRF52/SpiM.h"

namespace MTL {

template <uint32_t PIN_LUM = PIN_PAD_14,
          uint32_t PIN_CLK = PIN_PAD_13>
class PixelGen
{
public:
   PixelGen() = default;

   PixelGen(unsigned width_, unsigned height_, unsigned repeat_ = 1)
   {
      setSize(width_, height_, repeat_);
   }

   uint32_t getTaskStart() const { return spim.getTaskStart(); }

   //! Set pointer to frame buffer
   void setFramePtr(const uint8_t* ptr)
   {
      frame = ptr;
      setOffset(0);
   }

   //! Set frame size (pixels)
   //  Width must be a multiple of 8
   //  If repeat is zero scanning will be interlaced
   void setSize(unsigned width_, unsigned height_, unsigned repeat_ = 1)
   {
      repeat          = repeat_;
      bytes_per_line  = width_ / 8;
      bytes_per_frame = bytes_per_line * height_;
      interlace       = (repeat == 0);

      spim.setTxLength(bytes_per_line);

      unsigned pixel_freq = width_ >= 408 ? 16000000 :
                            width_ >= 208 ?  8000000 :
                            width_ >= 112 ?  4000000 :
                                             2000000;

      spim.setFrequency(pixel_freq);

      if (interlace)
      {
         repeat         = 1;
         bytes_per_line = bytes_per_line * 2;
      }
   }

   //! Set offset for top-left pixel from start of frame buffer (bytes)
   void setOffset(uintptr_t offset)
   {
      first_line = frame + offset;
   }

   //! Get ready for next field of image
   void startField(unsigned field)
   {
      repeat_counter = 0;
      line           = first_line;

      if (interlace && (field == 1))
      {
         line += bytes_per_line / 2;
      }
   }

   //! Start generating pixel data
   void startLine()
   {
      if (repeat_counter == 0)
      {
         repeat_counter = repeat - 1;

         line += bytes_per_line;
         if (line >= (frame + bytes_per_frame))
         {
            line -= bytes_per_frame;
         }
      }
      else
      {
         --repeat_counter;
      }
   }

   //! Stop generating pixel data
   void endLine()
   {
      spim.setTxData(line);
   }

private:
   // Emit pixel data from SPI master 3 MOSI
   MTL::SpiM3<PIN_CLK,PIN_LUM,nRF52::PIN_NULL> spim{};

   uint8_t                 repeat;
   uint8_t                 bytes_per_line;
   bool                    interlace;
   uint32_t                bytes_per_frame;
   const uint8_t*          frame{nullptr};
   const uint8_t*          first_line{nullptr};
   volatile const uint8_t* line{nullptr};
   volatile uint8_t        repeat_counter{0};
};

} // namespace MTL

#endif // MTL_MICROBIT_V2_PIXELGEN_H
