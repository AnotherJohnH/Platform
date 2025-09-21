//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Stub PLT::Yield implementation 

#include <sys/time.h>
#include <unistd.h>

#include "PLT/Yield.h"

const int32_t PLT::Yield::TICKS_PER_SEC = 1000000;

int32_t PLT::Yield::getTicks()
{
   struct timeval t;

   gettimeofday(&t, 0);

   return t.tv_sec * 1000000 + t.tv_usec;
}

void PLT::Yield::yieldForTicks(int32_t period_ticks)
{
   usleep(period_ticks);
}

