//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/Pins.h"
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

   signed download(unsigned sample_rate_hz_,
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
      setSampleRate(sample_rate_hz_);
      pio.SM_pinOUT(   sd, pin_sd_);
      pio.SM_pinSIDE(  sd, pin_sclk_lrclk_);
      pio.SM_configOSR(sd, CHANNELS * I2S_BITS_PER_SAMPLE, MTL::SHIFT_LEFT,
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

   void setSampleRate(unsigned sample_rate_hz_)
   {
      unsigned clock_freq = sample_rate_hz_ * CHANNELS * I2S_BITS_PER_SAMPLE * 2;

      pio.SM_clock(sd, clock_freq);
   }

private:
   static const unsigned CHANNELS            = 2;
   static const unsigned I2S_BITS_PER_SAMPLE = 16;
   static const unsigned X_INIT              = I2S_BITS_PER_SAMPLE - 2;

   PIO_TYPE pio{};
   int      sd{-1};
};

} // namespace MTL
