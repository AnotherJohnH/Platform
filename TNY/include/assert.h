//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#if defined(NDEBUG)

#define assert(expr)

#else

#include <stdlib.h>

#define assert(expr) do{ if (!(expr)) abort(); } while(1==0);

#endif // NDEBUG_H
