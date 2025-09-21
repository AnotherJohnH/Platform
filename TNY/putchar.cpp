//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

#include "MTL/MTL.h"

int putchar(int ch)
{
   MTL_putch(uint8_t(ch));
   return 0;
}
