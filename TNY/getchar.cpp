//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <cstdio>

#include "MTL/MTL.h"

int getchar()
{
   uint8_t ch = MTL_getch();
   if (ch == '\r')
      ch = '\n';
   return ch;
}
