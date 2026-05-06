//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file  PALVideo.h
//! \brief Generate a PAL video signal
//
//   Pad 15 is sync  => 330 ohm
//   Pad 14 is image => 150 ohm
//

#pragma once

#include "MTL/PixelGen.h"
#include "MTL/Digital.h"
#include "MTL/nRF52/Timer.h"
#include "MTL/nRF52/Ppi.h"
#include "MTL/nRF52/GpioTE.h"

#define PAL_VIDEO_ATTACH_IRQ(VIDEO) \
     extern "C" { void Timer_3_IRQ() { VIDEO.irqHandler(); } }

static const unsigned PAL_VIDEO_BYTE_SWAP = 0;


extern void PALVideo_FieldSync();


namespace MTL {

template <unsigned PIN_SYNC = PIN_PAD_15,
          unsigned PIN_LUM  = PIN_PAD_16>
class PALVideo
{
public:
   PALVideo(unsigned width_, unsigned height_)
   {
      // Use maximum timer resolution
      timer.setClock(MTL::nRF52::TIMER_16_MHZ);

      // 0th compare to trigger sync rising edge
      // 1st compare to trigger start of image
      // 2nd compare to trigger end of image
      // 3rd compare sets scan period and sync falling edge
      timer.setAction(0, H_LONG_SYNC,   0);
      timer.setAction(1, H_NO_IMAGE,    0);
      timer.setAction(2, H_NO_IMAGE,    0);
      timer.setAction(3, H_SYNC_PERIOD, MTL::nRF52::TIMER_IRQ | MTL::nRF52::TIMER_CLEAR);

      // Link compare events to sync pin set and clear
      unsigned index = gpio_te.allocTask(PIN_SYNC);
      ppi.link(timer.getEventCmp(0), gpio_te.getTaskSet(index), true);
      ppi.link(timer.getEventCmp(3), gpio_te.getTaskClr(index), true);

      // Link compare event to pixel_gen start task
      ppi.link(timer.getEventCmp(1), pixel_gen.getTaskStart(),  true);

      setSize(width_, height_);
   }

   void refresh()
   {
      // Nothing to do
   }

   //! Set pointer to frame buffer
   void setFramePtr(const uint8_t* ptr)
   {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
      pixel_gen.setFramePtr(ptr + 0x1F800000);
#pragma GCC diagnostic pop

      startLongSync(PAL_SYNC_PULSES);
      timer.start();
   }

   //! Set offset for top-left pixel from start of frame buffer (bytes)
   void setOffset(uintptr_t offset)
   {
      pixel_gen.setOffset(offset);
   }

   //! Set frame size (pixels)
   //! Width must be a multiple of 8
   void setSize(unsigned width_, unsigned height_)
   {
      unsigned max_image_lines = PAL_LINES - 2 * MIN_VERT_BORDER;
      unsigned v_scale;

      if (max_image_lines >= height_)
      {
         v_scale = max_image_lines / height_;

         // Size of image (scan lines)
         v_image = v_scale * height_;
      }
      else
      {
         // Interlace
         v_scale = 0;
         v_image = height_ / 2;
      }

      setVertPos(v_adjust);

      pixel_gen.setSize(width_, height_, v_scale);
   }

   //! Set horizontal position of image (1/16 uS)
   void setHorzPos(unsigned pos)
   {
      h_adjust = pos;
   }

   //! Set vertical position of image (scan lines)
   void setVertPos(unsigned lines)
   {
      v_adjust = lines;

      // Height of top border (scan lines)
      v_top    = ((PAL_LINES - v_image) / 2) + v_adjust;

      // Height of bottom border (scan lines)
      v_bottom = PAL_LINES - v_image - v_top;
   }

   //! entry point from Timer_0_IRQ
   void irqHandler()
   {
      if (line_type == LINE_IMAGE)
      {
         pixel_gen.endLine();
         pixel_gen.startLine();
      }

      lineEvent();

      timer.clrCompareEvent(3);
   }

private:
   void lineEvent()
   {
      if (--line_counter == 0)
      {
         if (line_type == LINE_SECOND_SHORT_SYNC)
         {
             line_type = LINE_LONG_SYNC;
         }
         else
         {
             ++line_type;
         }

         switch(line_type)
         {
         case LINE_LONG_SYNC:
            startLongSync(PAL_SYNC_PULSES);
            PALVideo_FieldSync();
            pixel_gen.startField(field);
            pixel_gen.startLine();
            field = field ^ 1;
            break;

         case LINE_FIRST_SHORT_SYNC:
            startShortSync(PAL_SYNC_PULSES - field);
            break;

         case LINE_TOP_BORDER:
            startBorder(v_top);
            break;

         case LINE_IMAGE:
            startImage(v_image);
            break;

         case LINE_BOTTOM_BORDER:
            startBorder(v_bottom);
            break;

         case LINE_SECOND_SHORT_SYNC:
            startShortSync(PAL_SYNC_PULSES + field);
            break;
         }
      }
   }

   //! Start sending long sync pulses
   void startLongSync(unsigned n)
   {
      timer.setCompare(0, H_LONG_SYNC);
      //timer.setCompare(1, H_NO_IMAGE);     set by startBorder()
      //timer.setCompare(2, H_NO_IMAGE);     set by startBorder()
      //timer.setCompare(3, H_SYNC_PERIOD);  set by startShortSync()
      line_counter = n;
   }

   //! Start sending short sync pulses
   void startShortSync(unsigned n)
   {
      timer.setCompare(0, H_SHORT_SYNC);
      //timer.setCompare(1, H_NO_IMAGE);     set by startBorder()
      //timer.setCompare(2, H_NO_IMAGE);     set by startBorder()
      timer.setCompare(3, H_SYNC_PERIOD);
      line_counter = n;
   }

   //! Start sending empty image lines
   void startBorder(unsigned n)
   {
      timer.setCompare(0, H_LINE_SYNC);
      timer.setCompare(1, H_NO_IMAGE);
      timer.setCompare(2, H_NO_IMAGE);
      timer.setCompare(3, H_LINE_PERIOD);
      line_counter = n;
   }

   //! Start sending image lines
   void startImage(unsigned n)
   {
      //timer.setCompare(0, H_LINE_SYNC);    set by startBorder
      timer.setCompare(1, H_IMAGE     + h_adjust);
      timer.setCompare(2, H_END_IMAGE + h_adjust);
      //timer.setCompare(3, H_LINE_PERIOD);  set by startBorder
      line_counter = n;
   }

   // PAL standard
   static const unsigned PAL_LINES          = 305;    // Per frame
   static const unsigned PAL_SYNC_PULSES    = 5;
   static const unsigned PAL_LINE_PERIOD_NS = 64000;  // 64.0 uS
   static const unsigned PAL_SHORT_SYNC_NS  =  2000;  //  2.0 uS
   static const unsigned PAL_LONG_SYNC_NS   = 30000;  // 30.0 uS
   static const unsigned PAL_LINE_SYNC_NS   =  4700;  //  4.7 uS
   static const unsigned PAL_BACK_PORCH_NS  =  8000;  //  8.0 uS
   static const unsigned PAL_FRONT_PORCH_NS =  1500;  //  1.5 uS

   // Derived value
   static const unsigned PAL_IMAGE_START_NS = PAL_LINE_SYNC_NS + PAL_BACK_PORCH_NS;
   static const unsigned PAL_IMAGE_END_NS   = PAL_LINE_PERIOD_NS - PAL_FRONT_PORCH_NS;
   static const unsigned PAL_SYNC_PERIOD_NS = PAL_LINE_PERIOD_NS / 2;

   // Local settings
   static const unsigned TIMER_FREQ   = 16000000;             // 16 MHz
   static const unsigned TICKS_PER_US = TIMER_FREQ / 1000000;
   static const unsigned V_ADJUST     = 10;                   // (lines)

   // Vertical constants
   static const unsigned MIN_VERT_BORDER = 10;                   // (lines)

   // Horizontal timing
   static const unsigned H_SYNC_PERIOD = (PAL_SYNC_PERIOD_NS * TICKS_PER_US / 1000);
   static const unsigned H_LINE_PERIOD = (PAL_LINE_PERIOD_NS * TICKS_PER_US / 1000);
   static const unsigned H_LONG_SYNC   = PAL_LONG_SYNC_NS   * TICKS_PER_US / 1000;
   static const unsigned H_SHORT_SYNC  = PAL_SHORT_SYNC_NS  * TICKS_PER_US / 1000;
   static const unsigned H_LINE_SYNC   = PAL_LINE_SYNC_NS   * TICKS_PER_US / 1000;
   static const unsigned H_IMAGE       = PAL_IMAGE_START_NS * TICKS_PER_US / 1000;
   static const unsigned H_END_IMAGE   = PAL_IMAGE_END_NS   * TICKS_PER_US / 1000;
   static const unsigned H_NO_IMAGE    = H_LINE_PERIOD + 1;

   enum LineType
   {
      LINE_LONG_SYNC,
      LINE_FIRST_SHORT_SYNC,
      LINE_TOP_BORDER,
      LINE_IMAGE,
      LINE_BOTTOM_BORDER,
      LINE_SECOND_SHORT_SYNC
   };

   MTL::nRF52::Timer3            timer;
   MTL::Digital::Out<PIN_SYNC>   sync;
   MTL::nRF52::Ppi               ppi;
   MTL::nRF52::GpioTE            gpio_te;
   MTL::PixelGen<PIN_LUM>        pixel_gen{};

   volatile uint8_t  field{0};
   volatile uint8_t  line_type{LINE_LONG_SYNC};
   volatile uint16_t line_counter{PAL_SYNC_PULSES};

   uint16_t v_adjust{V_ADJUST};
   uint16_t v_top;
   uint16_t v_image;
   uint16_t v_bottom;
   uint16_t h_adjust{0};
};

} // namespace MTL

