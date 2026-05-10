//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

extern "C" int __cxa_guard_acquire(volatile int* guard)
{
   // XXX not an atomic operation so not thread safe!
   return *guard == 0 ? 1 : 0;
}

extern "C" void __cxa_guard_release(volatile int* guard)
{
   *guard = 1;
}
