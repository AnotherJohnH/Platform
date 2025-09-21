//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

int __cxa_guard_acquire(volatile int* guard)
{
   return *guard == 0 ? 1 : 0;
}

void __cxa_guard_release(volatile int* guard)
{
   *guard = 1;
}
