//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

#include "MTL/MTL.h"

int fputc(int ch, FILE* fp)
{
   if ((fp == stdout) || (fp == stderr))
   {
      if (ch == '\n')
      {
         MTL_putch(uint8_t('\r'));
      }
      MTL_putch(uint8_t(ch));
      return ch;
   }

   return -1;
}
