//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// PWM implemented using the PIO (as per the RPio darasheet example)
// The actual PWM peripheral is most likely a better choice

#pragma once

#include "MTL/Pins.h"

#include "Pio.h"

namespace MTL {

template <typename PIO_TYPE>
class PioPwm : public PIO::AsmBase<1>
{
private:
   struct Code : public PIO::Asm
   {
      Code()
      {
         side_set(1, /* enable */ true);

         PIO::Lbl count_loop;
         PIO::Lbl no_set;
         PIO::Lbl skip;

         wrap_target();

            POP(PIO::NO_BLOCK)         .side(0);
            MOV(PIO::X, PIO::OSR);
            MOV(PIO::Y, PIO::ISR);
         lbl(count_loop);
            JMP(PIO::X_NE_Y, no_set);
            JMP(skip)                  .side(1);
         lbl(no_set);
            NOP();
         lbl(skip);
            JMP(PIO::Y_NE_Z_DEC, count_loop);

         wrap();
      }
   };

public:
   PioPwm(unsigned pin_, unsigned freq_, unsigned period_)
   {
      sd = pio.allocSM();
      if (sd < 0)
         return;

      // Write code to PIO
      Code code{};
      pio.SM_program(sd, code);

      // Configure state machine
      pio.SM_clock(    sd, freq_ * 3);
      pio.SM_pinSIDE(  sd, pin_);
      pio.SM_configOSR(sd, 32, MTL::SHIFT_LEFT,
                       MTL::MANUAL,
                       /* join_tx */ true);

      // Get the period into the ISR
      pio.SM_push(sd, period_);
      pio.SM_exec(sd, POP().op() );
      pio.SM_exec(sd, OUT(PIO::ISR, 32).op() );

      pio.start(1 << sd);
   }

   bool isOk() const { return sd >= 0; }

   uint32_t operator=(uint32_t value_) const
   {
      pio.SM_push(sd, value_);

      return value_;
   }

private:
   PIO_TYPE pio;
   int      sd;
};

} // namespace MTL
