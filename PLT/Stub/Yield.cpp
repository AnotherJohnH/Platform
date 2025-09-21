//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Stub PLT::Yield implementation 

#include <ctime>

#include "PLT/Yield.h"

const int32_t PLT::Yield::TICKS_PER_SEC = CLOCKS_PER_SEC;

int32_t PLT::Yield::getTicks()
{
   return clock();
}

void PLT::Yield::yieldForTicks(int32_t period_ticks)
{
   // No yield. This will lead to CPU hogging which
   // is likely not a problem for embedded targets.
}

