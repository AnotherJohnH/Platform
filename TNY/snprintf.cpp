//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>

int snprintf(char* buffer, size_t n, const char* format, ...)
{
   va_list ap;
   va_start(ap, format);
   int retval = vsnprintf(buffer, n, format, ap);
   va_end(ap);
   return retval;
}
