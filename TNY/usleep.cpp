//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <unistd.h>

#include "MTL/MTL.h"

int usleep(unsigned microseconds)
{
   if (microseconds > 0)
   {
      uint32_t future = MTL_us_clock() + microseconds + 1;

      while(int32_t(MTL_us_clock() - future) < 0);
   }

   return 0;
}
