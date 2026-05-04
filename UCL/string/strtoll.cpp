//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

#include "strto_template.h"

long long strtoll(const char* s, const char** endptr, int base)
{
   return UCL::strto<long long>(s, endptr, base);
}
