//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// NOTE: When streaming audio format should be a stereo pair of unsigned 16
//       bit integers with max level 2^BITS - 1

#pragma once

#include "MTL/chip/Pll.h"
#include "MTL/chip/Pwm.h"

#include "MTL/chip/rp2xxx/Audio.h"

namespace MTL {

//! Audio driver for PWM audio
template <unsigned PIN, unsigned BITS, unsigned BUFFER_SIZE = 1024, unsigned IRQ = 0>
class PwmAudio : public Audio::Base<BUFFER_SIZE, IRQ>
{
public:
   PwmAudio(unsigned sample_freq)
   {
      unsigned divider8_4 = PllSys().getFreq() / (sample_freq * (LIMIT >> 4));

      pwm.setSysClkDiv8_4(divider8_4);
   }

   void start()
   {
      this->startDMA(pwm.getDREQ(), pwm.getOut());
   }

   //! Manually push samples
   void push(int16_t left_, int16_t right_)
   {
      uint32_t left  = OFFSET + (left_  >> (16 - BITS));
      uint32_t right = OFFSET + (right_ >> (16 - BITS));

      pwm = (right << 16) | left;
   }

private:
   static const uint32_t LIMIT  = 1 << BITS;
   static const uint32_t OFFSET = LIMIT / 2;

   Pwm<PIN,/* PAIR */ true> pwm{/* clock_div_8_4 */ 0b10000, LIMIT};
};

} // namespace MTL
