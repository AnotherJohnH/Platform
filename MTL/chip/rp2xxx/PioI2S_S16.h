//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

#pragma once

#include "MTL/chip/Pio.h"

namespace MTL {

//! Output peripheral for I2S with 16-bit stereo samples
template <typename PIO_TYPE>
class PioI2S_S16 : public PIO::Asm
{
public:
   PioI2S_S16()
   {
      PIO::Lbl left_loop;
      PIO::Lbl right_loop;

      side_set(2);

      wrap_target();

      //###############################################################
      // OSR = R:L
      lbl(left_loop);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_0_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop)  .side(S_0_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_1_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)              .side(S_1_1);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:R
      lbl(right_loop);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_1_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop) .side(S_1_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_0_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)              .side(S_0_1);
      //---------------------------------------------------------------

      wrap();
   }

   signed download(unsigned clock_freq,
                   unsigned pin_sd,
                   unsigned pin_lrclk_sclk)
   {
      // Allocate a state machine
      sd = pio.allocSM();
      if (sd < 0)
         return -1;

      // Write code to PIO
      if (pio.SM_program(sd, *this) < 0)
         return -1;

      // Configure state machine
      pio.SM_clock(    sd, clock_freq);
      pio.SM_pinOUT(   sd, pin_sd);
      pio.SM_pinSIDE(  sd, pin_lrclk_sclk);
      pio.SM_configOSR(sd, I2S_BITS_PER_SAMPLE * 2, MTL::SHIFT_LEFT,
                       MTL::AUTO_PULL, /* join_tx */ true);

      unsigned x_init = X_INIT - 2;
      pio.SM_exec(sd, SET(PIO::X, x_init).side(S_0_0).op() );

      return sd;
   }

   void start()
   {
      pio.start(1 << sd);
   }

   void push(uint32_t packed_)
   {
      pio.SM_push(sd, packed_);
   }

   void push(int16_t left_, int16_t right_)
   {
      uint32_t packed = (left_ << 16) | (right_ & 0xFFFF);

      push(packed);
   }

private:
   static const unsigned I2S_BITS_PER_SAMPLE = 16;
   static const unsigned X_INIT              = I2S_BITS_PER_SAMPLE - 2;

   static const uint8_t S_0_0{0b00};
   static const uint8_t S_0_1{0b01};
   static const uint8_t S_1_0{0b10};
   static const uint8_t S_1_1{0b11};

   PIO_TYPE pio{};
   int      sd{-1};
};

} // namespace MTL
