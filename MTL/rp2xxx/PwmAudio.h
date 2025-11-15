//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// NOTE: When streaming audio format should be a stereo pair of unsigned 16
//       bit integers with max level 2^BITS - 1

#pragma once

#include "MTL/chip/Pll.h"
#include "MTL/chip/Pwm.h"

#include "MTL/rp2xxx/Audio.h"

namespace MTL {

//! Audio driver for PWM audio
template <unsigned PIN, unsigned BITS, unsigned BUFFER_SIZE = 1024, unsigned IRQ = 0>
class PwmAudio : public Audio::Base<BUFFER_SIZE, IRQ>
{
public:
   PwmAudio(unsigned sample_freq)
   {
      unsigned divider8_4 = PllSys().getFreq() / (sample_freq * (LIMIT >> 4));

      rate.setSysClkDiv8_4(divider8_4);
   }

   void start()
   {
      this->startDMA(rate.getDREQ(), pwm.getOut());
   }

   //! Re-format a pair of 16-bit signed samples for the PWM
   static uint32_t packSamples(int16_t left_, int16_t right_)
   {
      uint32_t left  = OFFSET + (left_  >> (16 - BITS));
      uint32_t right = OFFSET + (right_ >> (16 - BITS));

      return (right << 16) | left;
   }

   //! Manually push samples
   void push(int16_t left_, int16_t right_)
   {
      pwm = packSamples(left_, right_);
   }

private:
   static const uint32_t LIMIT  = 1 << BITS;
   static const uint32_t OFFSET = LIMIT / 2;

   Pwm<PIN,/* PAIR */ true> pwm{/* clock_div_8_4 */ 0b10000, LIMIT};
   Pwm<PIN + 2>             rate{/* clock_div_8_4 */ 0b10000, LIMIT, /* dreq only */ true};
};

} // namespace MTL
