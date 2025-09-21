//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdlib.h>
#include "strto.h"

unsigned long long strtoull(const char* s, const char** endptr, int base)
{
   return TNY::strto<unsigned long long>(s, endptr, base);
}
