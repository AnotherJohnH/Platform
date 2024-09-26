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

// PWM implemented using the PIO (as per the RPio darasheet example)
// The actual PWM peripheral is most likely a better choice

#pragma once

#include "Pio.h"
#include "MTL/Pins.h"

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
