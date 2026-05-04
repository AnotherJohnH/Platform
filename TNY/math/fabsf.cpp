//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <math.h>
#include <stdint.h>

float fabsf(float x)
{
   return x < 0.0 ? -x : x;
}
