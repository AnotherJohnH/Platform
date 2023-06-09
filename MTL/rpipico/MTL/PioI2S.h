//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

#include "rp2040/Pio.h"

namespace MTL {

class PioI2S : public PIO::Asm
{
public:
   PioI2S()
   {
      PIO::Lbl left_loop;
      PIO::Lbl right_loop;

      side_set(2);

      wrap_target();

      lbl(left_loop);
         OUT(PIO::PINS, 1)                 .side(0b00);
         JMP(PIO::X_NE_Z_DEC, left_loop)   .side(0b10);
         OUT(PIO::PINS, 1)                 .side(0b01);
         SET(PIO::X, 14)                   .side(0b11);

      lbl(right_loop);
         OUT(PIO::PINS, 1)                 .side(0b01);
         JMP(PIO::X_NE_Z_DEC, right_loop)  .side(0b11);
         OUT(PIO::PINS, 1)                 .side(0b00);
      entry();
         SET(PIO::X, 14)                   .side(0b10);

      wrap();
   }

   template <typename TYPE>
   signed download(TYPE&    pio,
                   unsigned sample_freq,
                   unsigned pin_sd,
                   unsigned pin_lrclk)
   {
       // Allocate a state machine
       signed sd = pio.allocSM();
       if (sd < 0)
          return sd;

       // Write code to PIO
       pio.SM_program(sd, *this);

       // Configure state machine
       pio.SM_clock(    sd, sample_freq * 32 * 2);
       pio.SM_pinOUT(   sd, pin_sd);
       pio.SM_pinSIDE(  sd, pin_lrclk, 2);
       pio.SM_configOSR(sd, 32, MTL::SHIFT_LEFT, MTL::AUTO_PULL, /* join_tx */ true);

       return sd;
   }
};

}
