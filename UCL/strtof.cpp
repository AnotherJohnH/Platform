//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

float strtof(const char* s, const char** endptr)
{
   return float(strtod(s, endptr));
}
