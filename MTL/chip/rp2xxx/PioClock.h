//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Pio.h"

namespace MTL {

template <typename TYPE>
class PioClock : public PIO::Asm
{
public:
   PioClock()
   {
      wrap_target();
         SET(PIO::PINS, 0);
         SET(PIO::PINS, 1);
      wrap();
   }

   signed download(unsigned freq_, unsigned pin_)
   {
      // Allocate a state machine
      sd = pio.allocSM();
      if (sd < 0)
         return sd;

      // Write code to PIO
      pio.SM_program(sd, *this);

      // Configure state machine
      setClock(freq_);
      pio.SM_pinSET(sd, pin_);

      return sd;
   }

   void start()
   {
      pio.start(1 << sd);
   }

   void setClock(unsigned freq_)
   {
      pio.SM_clock(sd, freq_ * 2);
   }

private:
   signed sd{-1};
   TYPE   pio;
};

}
