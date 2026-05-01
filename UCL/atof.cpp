//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

double atof(const char* s)
{
   return strtod(s, nullptr);
}
