//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <string.h>

int strcmp(const char* lhs, const char* rhs)
{
   while((*lhs == *rhs) && (*rhs != '\0'))
   {
      ++rhs;
      ++lhs;
   }

   return *lhs - *rhs;
}
