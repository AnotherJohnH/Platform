//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

char *strcpy(char* dest, const char* src)
{
   char* d = dest;

   while(*src)
   {
      *d++ = *src++;
   }

   *d = '\0';

   return dest;
}
