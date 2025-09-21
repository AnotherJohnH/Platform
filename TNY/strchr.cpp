//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

char *strchr(const char* s, int c)
{
   while(*s != c)
   {
      if (*s == '\0') return (char*)0;
      ++s;
   }

   return (char*)s;
}
