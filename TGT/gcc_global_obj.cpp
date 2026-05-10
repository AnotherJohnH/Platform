//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

extern "C" {

void* __dso_handle{nullptr};

int __aeabi_atexit(void* object,
                   void  (*destructor)(void*),
                   void* dso_handle)
{
   // ignored for now
   return 0;
}

}
