//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

char *strncpy(char* dest, const char* src, size_t n)
{
   char* d = dest;

   while((*src) && (n-- > 0))
   {
      *d++ = *src++;
   }

   *d = '\0';

   return dest;
}
