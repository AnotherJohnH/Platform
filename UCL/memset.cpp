//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <string.h>

void* memset(void* dest, int c, size_t n)
{
   while(n--)
   {
      ((char*)dest)[n] = c;
   }

   return dest;
}
