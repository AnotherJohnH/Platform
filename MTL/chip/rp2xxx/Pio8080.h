//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// \brief PIO for an 8080 style 8-bit data bus (write only)

#pragma once

#include "MTL/Pins.h"

#include "Pio.h"

namespace MTL {

class Pio8080 : public PIO::Asm
{
public:
   Pio8080()
   {
      side_set(1);

      wrap_target();
         OUT(PIO::PINS, 8).side(0);
         NOP()            .side(1);
      wrap();
   }

   template <typename TYPE>
   signed download(TYPE&    pio,
                   unsigned freq,
                   unsigned pin_db,
                   unsigned pin_wr)
   {
      // Allocate a state machine
      signed sd = pio.allocSM();
      if (sd < 0)
         return sd;

      // Write code to PIO
      pio.SM_program(sd, *this);

      // Configure state machine
      pio.SM_clock(  sd, freq);
      pio.SM_pinOUT( sd, pin_db, 8);
      pio.SM_pinSIDE(sd, pin_wr);

      return sd;
   }
};

} // namespace MTL
