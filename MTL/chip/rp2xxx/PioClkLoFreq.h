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

#pragma once

#include "Pio.h"

namespace MTL {


//! Clock good for low frequencies (< 1 Mhz)
template <typename PIO_TYPE, unsigned PIN>
class PioClkLoFreq : private PIO::AsmBase<1>
{
public:
   PioClkLoFreq(unsigned freq8_ = 0x100 /* 1 Hz */)
      : tick_freq(PllSys().getFreq())
   {
      // Allocate a state machine
      sd = pio.allocSM();
      if (sd < 0)
         return;

      struct Code : public PIO::Asm
      {
         Code()
         {
            side_set(1, /* enable */ true);

            PIO::Lbl one_loop;
            PIO::Lbl zero_loop;

            wrap_target();

               MOV(PIO::X, PIO::OSR)      .side(1);
               NOP();
            lbl(one_loop);
               JMP(PIO::X_NE_Z_DEC, one_loop);

               MOV(PIO::X, PIO::OSR)      .side(0);
               POP(PIO::NO_BLOCK);
            lbl(zero_loop);
               JMP(PIO::X_NE_Z_DEC, zero_loop);

            wrap();
         }

      } code;

      // Configure state machine
      pio.SM_clock(sd, tick_freq);
      pio.SM_program(sd, code);
      pio.SM_pinSIDE(sd, PIN);
      pio.SM_configOSR(sd, 32, MTL::SHIFT_LEFT,
                       MTL::MANUAL,
                       /* join_tx */ true);

      // Prime SM with an initial tick count
      setFreq8(freq8_);
      pio.SM_exec(sd, POP().op() );

      pio.start(1 << sd);
   }

   void setFreq8(unsigned freq8_)
   {
      pio.SM_push(sd, calcTicks(freq8_));
   }

   unsigned operator=(unsigned freq_)
   {
      setFreq8(freq_);
      return freq_;
   }

private:
   unsigned calcTicks(unsigned freq8_)
   {
      return (uint64_t(tick_freq) * 128 / freq8_) - 3;
   }

   const unsigned tick_freq;
   PIO_TYPE       pio;
   signed         sd;
};

}
