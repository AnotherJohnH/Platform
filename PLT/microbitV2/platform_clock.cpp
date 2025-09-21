//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/core/CortexM4/SysTick.h"

static MTL::SysTick      sys_tick{64000}; // 1ms ticks with a 64 MHz CPU clock
static volatile uint32_t ms_ticks{0};

extern "C" {

uint32_t MTL_ms_ticks() { return ms_ticks; }

void VEC_sysTick()
{
   ++ms_ticks;
}

}
