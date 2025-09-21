//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "Timer.h"

//! Return 100 Hz clock value
uint32_t MTL_clock()
{
   MTL::Timer0 us_timer;

   return us_timer.read64() / 10000;
}

//! Return 1 MHz clock value
uint32_t MTL_us_clock()
{
   MTL::Timer0 us_timer;

   return us_timer.read32();
}
