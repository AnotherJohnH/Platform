//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "Clock.h"

void MTL_init()
{
   // Start the 16MHz xtal
   MTL::Clock().startHFClock();
}
