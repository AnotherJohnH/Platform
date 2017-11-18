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

// \file PALVideo.h
// \brief NXP LPC1768 Generates PAL Video signal from a frame buffer
//
// mbed LPC1768 connections...
//   Pad  8 is sync  => 750 ohm
//   Pad 14 is video => 220 ohm
//   Pad  6 => Pad 16  (SPI select)
//   Pad  7 => Pad 13  (pixel clock)

#ifndef LPC1768_PAL_VIDEO_H
#define LPC1768_PAL_VIDEO_H


#include "MTL/chip/LPC1768/PWM.h"
#include "PixelGen.h"


#define PAL_VIDEO_ATTACH_IRQ(VIDEO) \
     extern "C" { void irqPWM1() { VIDEO.irqHandler(); } }


extern void PALVideo_FieldSync();


namespace MTL {

static const unsigned PAL_VIDEO_BYTE_SWAP = 1;

template <unsigned WIDTH, unsigned HEIGHT>
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
   static const unsigned V_SCALE  = (PAL_LINES - 2 * MIN_VERT_BORDER) / HEIGHT;

   // Horizontal timing
   static const unsigned H_SYNC_PERIOD = PAL_SYNC_PERIOD_US * TICKS_PER_US;
   static const unsigned H_LINE_PERIOD = PAL_LINE_PERIOD_US * TICKS_PER_US;
   static const unsigned H_SHORT_SYNC  = PAL_SHORT_SYNC_NS  * TICKS_PER_US / 1000;
   static const unsigned H_LONG_SYNC   = PAL_LONG_SYNC_NS   * TICKS_PER_US / 1000;
   static const unsigned H_LINE_SYNC   = PAL_LINE_SYNC_NS   * TICKS_PER_US / 1000;
   static const unsigned H_IMAGE       = PAL_IMAGE_START_NS * TICKS_PER_US / 1000;
   static const unsigned H_NO_IMAGE    = H_LINE_PERIOD + 1;

   enum LineType
   {
      LINE_FIRST_SHORT_SYNC  = 0,
      LINE_LONG_SYNC         = 1,
      LINE_SECOND_SHORT_SYNC = 2,
      LINE_TOP_BORDER        = 3,
      LINE_IMAGE             = 4,
      LINE_BOTTOM_BORDER     = 5
   };

   PWM                pwm;
   PixelGen           pixel_gen;

   volatile uint8_t   field;
   volatile uint8_t   line_type;
   volatile uint16_t  line_counter;

   uint16_t           v_top;
   uint16_t           v_adjust;
   uint16_t           v_image;
   uint16_t           v_bottom;

   int16_t            h_adjust;

   void lineFirstShortSync()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_LONG_SYNC);
         line_type    = LINE_LONG_SYNC;
         line_counter = PAL_LONG_SYNC_LINES;
      }
   }

   void lineLongSync()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_SHORT_SYNC);
         line_type    = LINE_SECOND_SHORT_SYNC;
         line_counter = PAL_SHORT_SYNC_LINES - field;
      }
   }

   void lineSecondShortSync()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_LINE_SYNC);
         pwm.setPeriod(H_LINE_PERIOD);
         line_type    = LINE_TOP_BORDER;
         line_counter = v_top;
      }
   }

   void lineTopBorder()
   {
      if (--line_counter == 0)
      {
         line_type    = LINE_IMAGE;
         line_counter = v_image;
         pixel_gen.startField();
         pwm.setIRQ<1>(H_IMAGE + h_adjust);
      }
   }

   void lineImage()
   {
      pixel_gen.endLine();
      if (--line_counter == 0)
      {
         pwm.setIRQ<1>(H_NO_IMAGE);
         line_type    = LINE_BOTTOM_BORDER;
         line_counter = v_bottom;
      }
   }

   void lineBottomBorder()
   {
      if (--line_counter == 0)
      {
         pwm.setRise(H_SHORT_SYNC);
         pwm.setPeriod(H_SYNC_PERIOD);
         line_type    = LINE_FIRST_SHORT_SYNC;
         line_counter = (PAL_SHORT_SYNC_LINES + field);
         field = field ^ 1;
         PALVideo_FieldSync();
      }
   }

   void lineEvent()
   {
      switch(line_type)
      {
      case LINE_FIRST_SHORT_SYNC:  lineFirstShortSync();  break;
      case LINE_LONG_SYNC:         lineLongSync();        break;
      case LINE_SECOND_SHORT_SYNC: lineSecondShortSync(); break;
      case LINE_TOP_BORDER:        lineTopBorder();       break;
      case LINE_IMAGE:             lineImage();           break;
      case LINE_BOTTOM_BORDER:     lineBottomBorder();    break;
      default: break;
      }
   }

public:
   PALVideo()
      : pwm(1)
      , pixel_gen(WIDTH, HEIGHT, V_SCALE)
      , field(0)
      , line_type(LINE_FIRST_SHORT_SYNC)
      , line_counter(1)
      , v_adjust(V_ADJUST)
      , h_adjust(0)
   {
      pwm.setRise(H_LINE_SYNC);
      pwm.setFall(1);
      pwm.setIRQ<0>(H_LINE_PERIOD);

      // Initialise timing
      resize(WIDTH, HEIGHT);

      // TODO don't start until a frame pointer has been set
      pwm.start();
   }

   //! entry point from Timer_0_IRQ
   void irqHandler()
   {
      if (pwm.isIRQ<1>())
      {
         pixel_gen.startLine();
         return;
      }
     
      lineEvent();

      pwm.isIRQ<0>();
   }

   //! Set pointer to frame buffer
   void setFramePtr(const uint8_t* ptr)
   {
      pixel_gen.setFramePtr(ptr);
   }

   //! Set offset for top-left pixel from start of frame buffer (bytes)
   void setOffset(uintptr_t offset)
   {
      pixel_gen.setOffset(offset);
   }

   //! Set frame width and height (pixels)
   //
   //!  \param width Must be a multiple of 32
   void resize(unsigned width, unsigned height)
   {
      pixel_gen.resize(width, height, V_SCALE);

      // Size of image (scan lines)
      v_image  = V_SCALE * height;

      // Height of top border (scan lines)
      v_top    = ((PAL_LINES - v_image) / 2) + v_adjust;

      // Height of bottom border (scan lines)
      v_bottom = PAL_LINES - v_image - v_top;
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
      //setHeight(v_image / V_SCALE);
   }
};


} // namespace MTL

#endif // LPC1768_PAL_VIDEO_H
