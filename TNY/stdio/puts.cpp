//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <stdio.h>

int puts(const char* s)
{
   while(*s)
   {
      putchar(*s++);
   }

   putchar('\n');

   return 0;
}
