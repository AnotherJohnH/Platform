//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

void* memcpy(void* dest, const void* src, size_t n)
{
   while(n--)
   {
      ((char*)dest)[n] = ((char*)src)[n];
   }

   return dest;
}
