//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <cstdio>
#include <cstdint>

#include "MTL/MTL.h"

int putchar(int ch)
{
   if (ch == '\n')
   {
      MTL_putch(uint8_t('\r'));
   }
   MTL_putch(uint8_t(ch));
   return ch;
}
