//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

#include "MTL/MTL.h"

int fgetc(FILE* fp)
{
   if (fp == stdin)
      return MTL_getch();
   
   return -1;
}
