//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

extern "C" void MTL_fault(unsigned long, void*);

extern "C" void __cxa_pure_virtual()
{
   MTL_fault(0x100, nullptr);
}
