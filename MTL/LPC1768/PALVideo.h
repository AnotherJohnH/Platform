//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \file PALVideo.h
// \brief NXP LPC1768 Generates PAL Video signal from a frame buffer
//
// mbed LPC1768 connections...
//   Pad  8 is sync  => 750 ohm
//   Pad 14 is video => 220 ohm
//   Pad  6 => Pad 16  (SPI select)
//   Pad  7 => Pad 13  (pixel clock)

#pragma once


#include "MTL/LPC1768/PWM.h"
#include "PixelGen.h"


#define PAL_VIDEO_ATTACH_IRQ(VIDEO) \
     extern "C" { void irqPWM1() { VIDEO.irqHandler(); } }


extern void PALVideo_FieldSync();


namespace MTL {

static const unsigned PAL_VIDEO_BYTE_SWAP = 1;

class PALVideo
{
private:
   // PAL standard
   static const unsigned PAL_LINES            = 305;
   static const unsigned PAL_SHORT_SYNC_LINES = 5;
   static const unsigned PAL_LONG_SYNC_LINES  = 5;
   static const unsigned PAL_SYNC_PERIOD_US   = 32;
   static const unsigned PAL_LINE_PERIOD_US   = 64;
   static const unsigned PAL_SHORT_SYNC_NS    =  2000;  //  2.0 uS
   static const unsigned PAL_LONG_SYNC_NS     = 30000;  // 30.9 uS
   static const unsigned PAL_LINE_SYNC_NS     =  4700;  //  4.7 uS
   static const unsigned PAL_IMAGE_START_NS   = 13500;  // 13.5 uS
   static const unsigned PAL_FRONT_PORCH_NS   =  1500;  //  1.5 uS

   // Local settings
   static const unsigned TIMER_FREQ     = 96000000;             // 96 MHz
   static const unsigned TICKS_PER_US   = TIMER_FREQ / 1000000;
   static const unsigned V_ADJUST       = 10;                   // (lines)

   // Vertical constants
   static const unsigned MIN_VERT_BORDER = 10;                   // (lines)

   // Horizontal timing
   static const unsigned H_SYNC_PERIOD = PAL_SYNC_PERIOD_US * TICKS_PER_US;
   static const unsigned H_LINE_PERIOD = PAL_LINE_PERIOD_US * TICKS_PER_US;
   static const unsigned H_SHORT_SYNC  = PAL_SHORT_SYNC_NS  * TICKS_PER_US / 1000;
   static const unsigned H_LONG_SYNC   = PAL_LONG_SYNC_NS   * TICKS_PER_US / 1000;
   static const unsigned H_LINE_SYNC   = PAL_LINE_SYNC_NS   * TICKS_PER_US / 1000;
   static const unsigned H_IMAGE       = PAL_IMAGE_START_NS * TICKS_PER_US / 1000;
   static const unsigned H_NO_IMAGE    = H_LINE_PERIOD + 1;

   enum class LineType : uint8_t
   {
      FIRST_SHORT_SYNC  = 0,
      LONG_SYNC         = 1,
      SECOND_SHORT_SYNC = 2,
      TOP_BORDER        = 3,
      IMAGE             = 4,
      BOTTOM_BORDER     = 5
   };

   PWM                pwm;
   PixelGen           pixel_gen;

   volatile uint8_t   field{0};
   volatile LineType  line_type{LineType::FIRST_SHORT_SYNC};
   volatile uint16_t  line_counter{1};

   uint16_t           v_top;
   uint16_t           v_adjust{V_ADJUST};
   uint16_t           v_image;
   uint16_t           v_bottom;

   int16_t            h_adjust{0};

   uint16_t           height;

   void lineFirstShortSync()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_LONG_SYNC);
         line_type    = LineType::LONG_SYNC;
         line_counter = PAL_LONG_SYNC_LINES;
      }
   }

   void lineLongSync()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_SHORT_SYNC);
         line_type    = LineType::SECOND_SHORT_SYNC;
         line_counter = PAL_SHORT_SYNC_LINES - field;
      }
   }

   void lineSecondShortSync()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_LINE_SYNC);
         pwm.setPeriod(H_LINE_PERIOD);
         line_type    = LineType::TOP_BORDER;
         line_counter = v_top;
      }
   }

   void lineTopBorder()
   {
      if (--line_counter == 0)
      {
         line_type    = LineType::IMAGE;
         line_counter = v_image;
         pixel_gen.startField(field ^ 1);
         pwm.setIRQ<1>(H_IMAGE + h_adjust);
      }
   }

   void lineImage()
   {
      pixel_gen.endLine();
      if (--line_counter == 0)
      {
         pwm.setIRQ<1>(H_NO_IMAGE);
         line_type    = LineType::BOTTOM_BORDER;
         line_counter = v_bottom;
      }
   }

   void lineBottomBorder()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_SHORT_SYNC);
         pwm.setPeriod(H_SYNC_PERIOD);
         line_type    = LineType::FIRST_SHORT_SYNC;
         line_counter = PAL_SHORT_SYNC_LINES + field;
         field = field ^ 1;
         PALVideo_FieldSync();
      }
   }

   void lineEvent()
   {
      switch(line_type)
      {
      case LineType::FIRST_SHORT_SYNC:  lineFirstShortSync();  break;
      case LineType::LONG_SYNC:         lineLongSync();        break;
      case LineType::SECOND_SHORT_SYNC: lineSecondShortSync(); break;
      case LineType::TOP_BORDER:        lineTopBorder();       break;
      case LineType::IMAGE:             lineImage();           break;
      case LineType::BOTTOM_BORDER:     lineBottomBorder();    break;
      default: break;
      }
   }

public:
   PALVideo()
      : pwm(1)
   {
      pwm.setRise(H_LINE_SYNC);
      pwm.setFall(1);
      pwm.setIRQ<0>(H_LINE_PERIOD);
   }

   //! entry point from Timer_0_IRQ
   void irqHandler()
   {
      if (pwm.isIRQ<1>())
      {
         pixel_gen.startLine(v_image - line_counter);
         return;
      }
     
      lineEvent();

      pwm.isIRQ<0>();
   }

   //! Set pointer to frame buffer
   void setFramePtr(const uint8_t* ptr)
   {
      pixel_gen.setFramePtr(ptr);

      // start video generation
      pwm.start();
   }

   //! Set pointer to scaning pixel generator
   void setScanner(void (*scanner)(uint8_t*, uint16_t))
   {
      pixel_gen.setScanner(scanner, height);
   }

   //! Set offset for top-left pixel from start of frame buffer (bytes)
   void setOffset(uintptr_t offset)
   {
      pixel_gen.setOffset(offset);
   }

   //! Set frame width and height (pixels)
   //
   //!  \param width Must be a multiple of 32
   void resize(unsigned width, unsigned height_)
   {
      height = height_;

      unsigned v_scale = (PAL_LINES - 2 * MIN_VERT_BORDER) / height;

      pixel_gen.resize(width, height, v_scale);

      // Size of image (scan lines)
      v_image  = v_scale == 0 ? height / 2 : v_scale * height;

      setVertPos(v_adjust);
   }

   //! Set horizontal position of image (uS)
   void setHorzPos(int pos)
   {
      h_adjust = pos*16;
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
};


} // namespace MTL

