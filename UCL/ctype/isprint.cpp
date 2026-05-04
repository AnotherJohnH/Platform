//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "ctype.h"

int isprint(int c)
{
   return isgraph(c) || (c == ' ');
}
