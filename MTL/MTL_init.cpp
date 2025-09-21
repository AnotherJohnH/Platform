//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

__attribute__((weak))
void MTL_init()
{
}

__attribute__((weak))
void MTL_fault(unsigned excep_num_, uint32_t* fault_stack_ptr_)
{
   while(true);
}
