//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <string.h>

char* strchr(const char* s, int c)
{
   while(*s != c)
   {
      if (*s == '\0')
         return nullptr;
      ++s;
   }

   return (char*)s;
}
