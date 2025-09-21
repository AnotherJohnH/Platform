//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

__attribute__((weak))
void MTL_halt(uint32_t status)
{
   while(true);
}
