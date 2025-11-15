//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
