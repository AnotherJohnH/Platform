//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

#include "MTL/MTL.h"

int fputc(int c, FILE* fp)
{
   if (fp == stdout)
   {
      MTL_putch(uint8_t(c));
      return 0;
   }

   return -1;
}
