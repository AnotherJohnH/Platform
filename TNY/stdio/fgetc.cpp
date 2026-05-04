//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

int fgetc(FILE* fp)
{
   if (fp == stdin)
      return getchar();
   
   return -1;
}
