//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "string.h"

size_t strlen(const char* src)
{
   size_t size = 0;

   while(*src++)
   {
      ++size;
   }

   return size;
}
