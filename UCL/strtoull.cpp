//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

#include "strto_template.h"

unsigned long long strtoull(const char* s, const char** endptr, int base)
{
   return UCL::strtou<unsigned long long>(s, endptr, base);
}
