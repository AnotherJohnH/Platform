//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <math.h>

float powf(float x, float y)
{
   if (x <= 0.0f)
       return 0.0f;

   return expf(y * logf(x));
}
