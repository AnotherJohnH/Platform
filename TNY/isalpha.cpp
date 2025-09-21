//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <ctype.h>

int isalpha(int c)
{
   return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}
