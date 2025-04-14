//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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
         OUT(PIO::PINS, 1)                .side(0b00);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop)  .side(0b01);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(0b10);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)              .side(0b11);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:R
      lbl(right_loop);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(0b10);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop) .side(0b11);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(0b00);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)              .side(0b01);
      //---------------------------------------------------------------

      wrap();
   }

   signed download(unsigned clock_freq_,
                   unsigned pin_sd_,
                   unsigned pin_sclk_lrclk_)
   {
      // Allocate a state machine
      sd = pio.allocSM();
      if (sd < 0)
         return -1;

      // Write code to PIO
      if (pio.SM_program(sd, *this) < 0)
         return -1;

      // Configure state machine
      pio.SM_clock(    sd, clock_freq_);
      pio.SM_pinOUT(   sd, pin_sd_);
      pio.SM_pinSIDE(  sd, pin_sclk_lrclk_);
      pio.SM_configOSR(sd, I2S_BITS_PER_SAMPLE * 2, MTL::SHIFT_LEFT,
                       MTL::AUTO_PULL, /* join_tx */ true);

      unsigned x_init = X_INIT - 2;
      pio.SM_exec(sd, SET(PIO::X, x_init).side(0b00).op() );

      return sd;
   }

   void start()
   {
      pio.start(1 << sd);
   }

   //! Tx a packed 16-bit sample pair
   void push(uint32_t packed_)
   {
      pio.SM_push(sd, packed_);
   }

   //! Tx a 16-bit sample pair
   void push(int16_t left_, int16_t right_)
   {
      uint32_t packed = (left_ << 16) | (right_ & 0xFFFF);

      push(packed);
   }

private:
   static const unsigned I2S_BITS_PER_SAMPLE = 16;
   static const unsigned X_INIT              = I2S_BITS_PER_SAMPLE - 2;

   PIO_TYPE pio{};
   int      sd{-1};
};

} // namespace MTL
