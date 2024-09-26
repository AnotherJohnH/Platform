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
//
// \brief PIO for an 8080 style 8-bit data bus (write only)

#pragma once

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
