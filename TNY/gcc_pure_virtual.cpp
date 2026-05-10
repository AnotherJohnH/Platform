//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

extern "C" void __cxa_pure_virtual()
{
   MTL_fault(0x100, nullptr);
}
