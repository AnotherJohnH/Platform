//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/rpx2xxx/Pio.h"

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
