//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <math.h>

float tanhf(float x)
{
        if (x > +20.0f) return +1.0f;
   else if (x < -20.0f) return -1.0f;

   float exp_2x = expf(2.0f * x);
   return (exp_2x - 1.0f) / (exp_2x + 1.0f);
}
