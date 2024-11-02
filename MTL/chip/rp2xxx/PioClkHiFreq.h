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


//! Clock good for high frequencies (>= 1 Mhz)
template <typename PIO_TYPE, unsigned PIN>
class PioClkHiFreq
{
public:
   PioClkHiFreq(unsigned freq_)
   {
      // Allocate a state machine
      sd = pio.allocSM();
      if (sd < 0)
         return;

      struct Code : public PIO::Asm
      {
         Code()
         {
            wrap_target();
               SET(PIO::PINS, 0);
               SET(PIO::PINS, 1);
            wrap();
         }

      } code;

      pio.SM_program(sd, code);
      pio.SM_pinSET(sd, PIN);

      setFreq(freq_);

      pio.start(1 << sd);
   }

   void setFreq(unsigned freq_)
   {
      pio.SM_clock(sd, freq_ * 2);
   }

   unsigned operator=(unsigned freq_)
   {
      setFreq(freq_);
      return freq_;
   }

private:
   PIO_TYPE pio;
   signed   sd;
};

}
