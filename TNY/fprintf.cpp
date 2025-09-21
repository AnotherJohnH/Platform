//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

int fprintf(FILE* stream, const char* format, ...)
{
   va_list  ap;

   va_start(ap, format);
   vfprintf(stream, format, ap);
   va_end(ap);

   return 0;
}
