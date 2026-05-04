//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "ctype.h"

int ispunct(int c)
{
   return isgraph(c) && not isalnum(c);
}
