//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <string.h>

char *strcat(char* dest, const char* src)
{
   char* d = dest;

   while(*d)
   {
      ++d;
   }

   strcpy(d, src);

   return dest;
}
