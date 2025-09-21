//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

int strncmp(const char* lhs, const char* rhs, size_t n)
{
   while((*lhs++ == *rhs++) && (--n > 0))
   {
      if (*rhs == '\0') break;
   }

   return *lhs - *rhs;
}
