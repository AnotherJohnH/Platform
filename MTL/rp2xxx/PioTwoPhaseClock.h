//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/rp2xxx/Pio.h"

namespace MTL {

class PioTwoPhaseClock : public PIO::Asm
{
public:
   PioTwoPhaseClock()
   {
       side_set(1);

       wrap_target();
          SET(PIO::PINS, 0).side(1);
          SET(PIO::PINS, 1).side(0);
       wrap();
   }

   template <typename TYPE>
   signed download(TYPE& pio, unsigned freq, unsigned pin_a, unsigned pin_b)
   {
       // Allocate a state machine
       signed sd = pio.allocSM();
       if (sd < 0)
          return sd;

       // Write code to PIO
       pio.SM_program(sd, *this);

       // Configure state machine
       pio.SM_clock(  sd, freq * 2);
       pio.SM_pinSET( sd, pin_a); // Main output
       pio.SM_pinSIDE(sd, pin_b); // Inverted output

       return sd;
   }
};

}
