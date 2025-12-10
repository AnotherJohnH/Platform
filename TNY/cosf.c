//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <math.h>
#include <stdint.h>

float cosf(float x_)
{
   // Handle NaN or infinity
   union { float f; uint32_t u; } ix = {x_};
   if ((ix.u & 0x7f800000) == 0x7f800000)
      return x_ - x_;

   // Argument reduction
   float   k = x_ * M_1_PI_F;
   int32_t n = (int32_t)(k + (k >= 0.0f ? 0.5f : -0.5f));
   float   r = x_ - n * M_PI_F;

    // Map to [-pi/2, pi/2]
   if (r > M_PI_2_F)
      r = M_PI_F - r;
   else if (r < -M_PI_2_F)
      r = -M_PI_F - r;

   // Taylor series
   // cos(x) ~= 1 - x^2/2! + x^4/4! - x^6/6!
   const float C2 = -1.0f / 2;
   const float C4 = +1.0f / 24;
   const float C6 = -1.0f / 720;

   float r2     = r * r;
   float result = 1.0f + r2 * (C2 + r2 * (C4 + r2 * C6));

   return (n & 1) != 0 ? -result : result;
}
