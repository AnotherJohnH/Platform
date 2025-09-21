//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdlib.h>
#include "strto.h"

unsigned long strtoul(const char* s, const char** endptr, int base)
{
   return TNY::strto<unsigned long>(s, endptr, base);
}
