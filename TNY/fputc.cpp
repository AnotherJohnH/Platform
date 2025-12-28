//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <cstdio>

int fputc(int ch, FILE* fp)
{
   if ((fp == stdout) || (fp == stderr))
   {
      putchar(ch);
      return ch;
   }

   return -1;
}
