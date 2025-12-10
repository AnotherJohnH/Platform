//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <math.h>
#include <stdint.h>

float sinf(float x_)
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
   // sin(x) ~= x - x^3/3! + x^5/5! - x^7/7!
   const float C3 = -1.0f / 6;
   const float C5 = +1.0f / 120;
   const float C7 = -1.0f / 5040;

   float r2     = r * r;
   float r3     = r2 * r;
   float result = r + r3 * (C3 + r2 * (C5 + r2 * C7));

   return (n & 1) != 0 ? -result : result;
}
