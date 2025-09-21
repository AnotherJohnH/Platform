//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040 Watchdog peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct WatchdogReg
{
   uint32_t ctrl;
   uint32_t load;
   uint32_t reason;
   uint32_t scratch[8];
   uint32_t tick;
};

//! 1 MHz timer
class Watchdog
   : public Periph<WatchdogReg, 0x40058000>
{
public:
   Watchdog() = default;

   //! Start the tick clock for a specific reference clock
   void start(unsigned cycles)
   {
      reg->tick = TICK_ENABLE | cycles;
   }

private:
   static const uint32_t TICK_ENABLE = 1 << 9;
};

} // namespace MTL
