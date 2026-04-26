//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <ctype.h>

int isalnum(int c)
{
   return isalpha(c) || isdigit(c);
}
