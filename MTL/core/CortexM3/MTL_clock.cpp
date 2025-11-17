//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/chip/Config.h"

#include "SysTick.h"
#include "Vector.h"

static const unsigned TICK_PERIOD_MS = 10;       //!< 10 mS

static MTL::SysTick tick {CLOCK_FREQ * TICK_PERIOD_MS / 1000};

static volatile uint32_t ticks{0};

void VEC_sysTick()
{
   ++ticks;
}

uint32_t MTL_clock()
{
   return ticks;
}

uint32_t MTL_us_clock()
{
   return ticks * 10000;
}
