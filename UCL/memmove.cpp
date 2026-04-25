//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

void* memmove(void* dest, const void* src, size_t n)
{
   if (dest > src)
   {
      while(n--)
      {
         ((char*)dest)[n] = ((char*)src)[n];
      }
   }
   else
   {
      for(size_t i=0; i<n; i++)
      {
         ((char*)dest)[i] = ((char*)src)[i];
      }
   }

   return dest;
}
