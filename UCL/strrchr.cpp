//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <string.h>

char* strrchr(const char* s, int c)
{
   const char* m = nullptr;

   while(*s != '\0')
   {
      if (*s == c)
         m = s;
      ++s;
   }

   return (char*)m;
}
