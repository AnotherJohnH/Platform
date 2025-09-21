//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

char *strrchr(const char* s, int c)
{
   const char* m = (const char*)0;

   while(*s != '\0')
   {
      if (*s == c) m = s;
      ++s;
   }

   return (char*)m;
}
