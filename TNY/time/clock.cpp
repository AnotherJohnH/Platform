//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <time.h>

#include "MTL/MTL.h"

clock_t clock(void) { return MTL_clock(); }
