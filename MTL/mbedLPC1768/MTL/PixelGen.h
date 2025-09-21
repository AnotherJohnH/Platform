//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \file PixelGen.h
// \brief NXP LPC1768 Generates a stream of pixels using GP DMA and the I2S

#pragma once

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
   void           (*scanner)(uint8_t* buffer, uint16_t y) = nullptr;

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

   //! Set pointer to scaning pixel generator
   void setScanner(void (*scanner_)(uint8_t*, uint16_t), uint16_t height)
   {
      scanner = scanner_;

      if (scanner != nullptr)
         size = bytes_per_line * 2;
      else
         size = bytes_per_line * height;
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

      if (scanner)
      {
          (*scanner)((uint8_t*)next, 0);
      }
   }

   //! Start generating pixel data
   void startLine(uint16_t line)
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

         if (scanner)
         {
            (*scanner)((uint8_t*)next, line);
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

