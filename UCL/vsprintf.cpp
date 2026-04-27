//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include "sprintf.h"
#include "stdint.h"

int vsprintf(char* buffer, const char* format, va_list ap)
{
   return vsnprintf(buffer, UINT32_MAX, format, ap);
}
