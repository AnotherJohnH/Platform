//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <ctype.h>

int isspace(int c)
{
   return (c == ' ') || (c == '\n') || (c == '\t');
}
