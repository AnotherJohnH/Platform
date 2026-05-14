//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "MTL/nRF51/Timer.h"

static MTL::nRF51::Timer<0> us_timer;

//! Return 100 Hz clock value
uint32_t MTL_clock()
{
   return us_timer.getCount(0) / 10000;
}

//! Return 1 MHz clock value
uint32_t MTL_us_clock()
{
   return us_timer.getCount(0);
}
