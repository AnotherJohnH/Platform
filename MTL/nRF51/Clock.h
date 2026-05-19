//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief nRF52 Clock control
//
// Data source Nordic Semiconductor "nRF51 Series Reference Manual Version 3.0"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union Reg
{
   // Tasks
   REG(0x000, task_hfclkstart);
   REG(0x004, task_hfclkstop);
   REG(0x008, task_lfclkstart);
   REG(0x00C, task_lfclkstop);
   REG(0x010, task_cal);
   REG(0x014, task_ctstart);
   REG(0x018, task_ctstop);

   // Events
   REG(0x100, event_hfclkstarted);
   REG(0x104, event_lfclkstarted);
   REG(0x10C, event_done);
   REG(0x110, event_ctto);

   // Registers
   REG(0x304, intenset);
   REG(0x308, intenclr);
   REG(0x408, hfclkrun);
   REG(0x40C, hfclkstat);
   REG(0x414, lfclkrun);
   REG(0x418, lfclkstat);
   REG(0x41C, lfclksrccpy);
   REG(0x518, lfclksrc);
   REG(0x538, ctiv);
   REG(0x550, xtalfreq);
};

class Clock : public Periph<Reg,0x40000000>
{
public:
   Clock() = default;

   void startHFClock()
   {
      reg->xtalfreq        = 0xFF; // 16 MHz
      reg->task_hfclkstart = 1;

      while(reg->event_hfclkstarted == 0);
   }
};

} // namespace MTL
