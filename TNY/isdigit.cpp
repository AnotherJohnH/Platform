//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <ctype.h>

int isdigit(int c)
{
   return (c >= '0') && (c <= '9');
}
