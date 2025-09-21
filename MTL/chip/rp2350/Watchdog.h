//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2350 Watchdog peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct WatchdogReg
{
   uint32_t ctrl;
   uint32_t load;
   uint32_t reason;
   uint32_t scratch[8];
};

//! 1 MHz timer
class Watchdog
   : public Periph<WatchdogReg, 0x400d8000>
{
public:
   Watchdog() = default;

private:
};

} // namespace MTL
