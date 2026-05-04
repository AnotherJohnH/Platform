//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdio.h"

#include "stdint.h"

int vsprintf(char* buffer, const char* format, va_list ap)
{
   return vsnprintf(buffer, UINT32_MAX, format, ap);
}

int sprintf(char* buffer, const char* format, ...)
{
   va_list ap;
   int n;
   va_start(ap, format);
   n = vsprintf(buffer, format, ap);
   va_end(ap);
   return n;
}

