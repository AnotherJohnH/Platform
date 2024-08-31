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
// \brief RP2350 Tick peripheral

#pragma once

#include "MTL/Periph.h"

#include "XOsc.h"

namespace MTL {

struct TicksReg
{
   struct Gen
   {
      uint32_t ctrl;
      uint32_t cycles;
      uint32_t count;
   };

   Gen      proc0;
   Gen      proc1;
   Gen      timer0;
   Gen      timer1;
   Gen      watchdog;
   uint32_t pad[3];
};

//! PWM peripheral
class Ticks : public Periph<TicksReg, 0x40108000>
{
public:
   Ticks() = default;

   void start()
   {
      startGen(reg->proc0);
      startGen(reg->proc1);
      startGen(reg->timer0);
      startGen(reg->timer1);
      startGen(reg->watchdog);
   }

private:
   void startGen(volatile TicksReg::Gen& gen)
   {
      gen.cycles = XOsc::XTAL_FREQ_MHZ; // Note actual clock is clk_ref
      gen.ctrl   = 1;
   }
};

} // namespace MTL
