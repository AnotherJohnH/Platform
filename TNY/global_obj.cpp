//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

extern "C" {

void *__dso_handle;

int __aeabi_atexit(void (*destructor)(void*),
                   void* arg,
                   void* __dso_handle)
{
   return 0;
}

}
