//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdlib.h>
#include "strto.h"

long atol(const char* s)
{
   return TNY::strto<long>(s, nullptr, 10);
}
