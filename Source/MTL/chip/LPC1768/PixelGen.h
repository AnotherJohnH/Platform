//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

// \file PixelGen.h
// \brief NXP LPC1768 Generates a stream of pixels using GP DMA and the I2S

#ifndef LPC1768_PIXEL_GEN_H
#define LPC1768_PIXEL_GEN_H

#include  <cassert>

#include "MTL/chip/LPC1768/I2S.h"
#include "MTL/chip/LPC1768/GPDMA.h"

namespace MTL {


class PixelGen
{
private:
   static const unsigned CHAN = 0;

   // Device data
   I2S            i2s;
   GPDMA          gpdma;
   volatile DMA*  head;
   DMA            tail;
   uint32_t       null32{0};

   // Image date
   const uint8_t*             frame;
   const uint8_t*             start;
   unsigned                   width;
   uint8_t                    bytes_per_line;
   uint32_t                   size;
   volatile const uint8_t*    next;
   volatile uint8_t           row;
   uint8_t                    scan_repeat;
   uint8_t                    odd_field_offset;

public:
   PixelGen()
   {
      i2s.setTxFormat(8, /* mono */ false);
      i2s.setTxMute(true);
      i2s.configDMA(0, /* tx */ true, /* depth */ 4);

      // Configure DMA channel
      gpdma.configChan(CHAN, GPDMA::MEMORY, GPDMA::I2S_0);

      // Initialise head of DMA chain
      head = gpdma.getHead(CHAN);
      head->setDest(i2s.getTxFifoAddress());

      // Setup up end of line I2S flush
      tail.setSrc(&null32);
      tail.setDest(i2s.getTxFifoAddress());
      tail.setControl(DMA::BURST1, DMA::WIDTH32, false,
                      DMA::BURST1, DMA::WIDTH32, false,
                      1);
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

   //! Set frame size (pixels)
   //
   //! \param width should be a multiple of 32
   //! \param height (pixels)
   //! \param scan_repeat
   void resize(unsigned width_, unsigned height_, uint8_t scan_repeat_)
   {
      width          = (width_ + 0x1F) & ~0x1F;
      bytes_per_line = width/8;
      size           = bytes_per_line * height_;

      if (scan_repeat_ == 0)
      {
         // Interlace
         scan_repeat = 1;
         odd_field_offset = bytes_per_line;
         bytes_per_line = bytes_per_line * 2;
      }
      else
      {
         // No interlace
         scan_repeat = scan_repeat_;
         odd_field_offset = 0;
      }

      if (width > 640)
      {
         i2s.setTxMulDiv(1, 3);
      }
      else if (width > 320)
      {
         i2s.setTxMulDiv(1, 6);
      }
      else if (width > 160)
      {
         i2s.setTxMulDiv(1, 12);
      }
      else if (width > 80)
      {
         i2s.setTxMulDiv(1, 24);
      }
      else
      {
         i2s.setTxMulDiv(1, 48);
      }
   }

   //! Get ready for next field of image
   void startField(uint8_t field)
   {
      row  = 0;
      next = start + field * odd_field_offset;
      head->setSrc(next);
   }

   //! Start generating pixel data
   void startLine()
   {
      i2s.setTxMute(false);

      head->setNext(&tail);

      head->setControl(DMA::BURST4, DMA::WIDTH32, true,
                       DMA::BURST4, DMA::WIDTH32, false,
                       width/32);

      gpdma.start(CHAN);

      if (++row == scan_repeat)
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
      i2s.setTxMute(true);
      head->setSrc(next);
   }
};


} // namespace MTL

#endif // LPC1768_PIXEL_GEN_H
