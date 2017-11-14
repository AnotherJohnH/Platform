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
// \brief NXP LPC1768 Generates a stream of pixels

#ifndef LPC1768_PIXEL_GEN_H
#define LPC1768_PIXEL_GEN_H

#include  <cstring>

#include "I2S.h"
#include "GPDMA.h"


template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned SCAN_REPEAT = 1>
class PixelGen
{
private:
   static const unsigned CHAN = 0;

   I2S            i2s;
   GPDMA          gpdma;
   volatile DMA*  head;
   DMA            tail;
   uint32_t       null32;

   const uint8_t*             frame;
   const uint8_t*             start;
   unsigned                   width;
   uint8_t                    bytes_per_line;
   uint32_t                   size;
   volatile const uint8_t*    next;
   volatile uint8_t           row;

public:
   PixelGen()
      : null32(0)
   {
      setWidth(WIDTH);

      i2s.setTxFormat(8, /* mono */ false);
      i2s.setTxMute(true);
      i2s.configDMA(0, /* tx */ true, /* depth */ 4);

      if (WIDTH > 640)
      {
         i2s.setTxMulDiv(1, 3);
      }
      else if (WIDTH > 320)
      {
         i2s.setTxMulDiv(1, 6);
      }
      else if (WIDTH > 160)
      {
         i2s.setTxMulDiv(1, 12);
      }
      else if (WIDTH > 80)
      {
         i2s.setTxMulDiv(1, 24);
      }
      else
      {
         i2s.setTxMulDiv(1, 48);
      }

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

   //! Set frame width (pixels)
   //  Must be less than or equal to WIDTH
   //  Must be a multiple of 32
   void setWidth(unsigned width_)
   {
      assert(width_ <= WIDTH);

      width          = (width_ + 0x1F) & ~0x1F;
      bytes_per_line = width/8;
      size           = bytes_per_line * HEIGHT;
   }

   //! Get ready for next field of image
   void startField()
   {
      row  = 0;
      next = start;
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
      i2s.setTxMute(true);
      head->setSrc(next);
   }
};


} // namespace MTL

#endif // LPC1768_PIXEL_GEN_H
