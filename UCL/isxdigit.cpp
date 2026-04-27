//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "ctype.h"

int isxdigit(int c)
{
   if (isdigit(c))
      return true;

   c = tolower(c);
   return (c >= 'a') && (c <= 'f');
}
