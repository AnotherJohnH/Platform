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
//! \brief Pixel stream generator for microbit

#ifndef MTL_MICROBIT_PIXEL_GEN_H
#define MTL_MICROBIT_PIXEL_GEN_H

#include  <cstring>

#include "MTL/Digital.h"
#include "MTL/ClockOut.h"
#include "MTL/nRF51/SpiSlave.h"

namespace MTL {

template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned SCAN_REPEAT     = 1,
          unsigned PIN_PIXEL_CLK   = PIN_PAD_7,
          unsigned PIN_SEL_SPI     = PIN_PAD_6,
          unsigned PIN_SPI_SEL     = PIN_PAD_16,
          unsigned PIN_SPI_MISO    = PIN_PAD_14,
          unsigned PIN_SPI_CLK     = PIN_PAD_13,
          unsigned PIXEL_CLK_TIMER = 1,
          unsigned GPIOTE          = 0,
          unsigned PPI             = 0>
class PixelGen
{
private:
   static const unsigned PIXEL_FREQ = WIDTH >= 256 ? 8000000 :
                                      WIDTH >= 128 ? 4000000 :
                                                     4000000; // TODO  2Mhz inop

   // Physically connect to PIN_SPI_CLK
   MTL::ClockOut<PIN_PIXEL_CLK,
                 PIXEL_CLK_TIMER,
                 GPIOTE,
                 PPI>                 pixel_clk;

   // Physically connect to PIN_SPI_SEL
   MTL::Digital::Out<PIN_SEL_SPI>     spi_select;

   // Pixeld data from PIN_SPI_MISO
   MTL::nRF51::SpiSlave<PIN_SPI_SEL,
                        PIN_NOT_CONNECTED, // MOSI not used
                        PIN_SPI_MISO,
                        PIN_SPI_CLK>  spi;

   const uint8_t*          frame;
   const uint8_t*          start;
   unsigned                width;
   uint8_t                 bytes_per_line;
   uint32_t                size;
   volatile const uint8_t* next;
   volatile uint8_t        row;

public:
   PixelGen()
      : pixel_clk(PIXEL_FREQ, /* start_now */ false)
      , spi_select(true)
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
      spi.setTxLength(width/8);
   }

   //! Start generating pixel data
   void startLine()
   {
      spi.release();
      spi_select = false;
      pixel_clk.start();
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
      pixel_clk.stop();
      spi_select = true;;
      spi.setTxPtr((const uint8_t*)next);
   }
};


} // namespace MTL

#endif // MTL_MICROBIT_PIXELGEN_H
