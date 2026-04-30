//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "MTL/LPC1768/SysCon.h"

void MTL_init()
{
   MTL::SysCon().init();
}
