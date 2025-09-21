//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdlib.h>
#include "strto.h"

int atoi(const char* s)
{
   return TNY::strto<int>(s, nullptr, 10);
}
