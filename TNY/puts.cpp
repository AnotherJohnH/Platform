//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

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
