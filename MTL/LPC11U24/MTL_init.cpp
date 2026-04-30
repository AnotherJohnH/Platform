//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "MTL/LPC11U24/SysCon.h"

void MTL_init()
{
   MTL::SysCon().init();
}
