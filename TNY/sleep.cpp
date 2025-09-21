//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <unistd.h>

#include "MTL/MTL.h"

int sleep(unsigned seconds)
{
   if (seconds > 0)
   {
       uint32_t future = MTL_clock() + seconds * 100 + 1;

       while(MTL_clock() < future);
   }

   return 0;
}
