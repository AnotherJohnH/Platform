//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

int atoi(const char* s)
{
   return int(strtol(s, nullptr, 10));
}
