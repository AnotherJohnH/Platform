//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <cmath>
#include <cstdint>

float logf(float x)
{
   if (x <= 0.0f)
       return -3.402823466e38f; // crude -inf substitute

   // Decompose float into exponent and mantissa
   union { float f; uint32_t i; } u;
   u.f = x;

   signed exponent = signed((u.i >> 23) & 0xFF) - 127;

   // Normalise mantissa to [1,2]
   u.i = (u.i & 0x7FFFFF) | (127 << 23);
   float m = u.f;

   // Now x = m * 2^exponent
   // log(x) = log(m) + exponent * ln(2)

   float r = m - 1.0f; // m in [1,2) so r in [0,1)

   // 5th-order Taylor for ln(1+r)
   float r2 = r * r;
   float r3 = r2 * r;
   float r4 = r3 * r;
   float r5 = r4 * r;

   float log_m = r - r2 * 0.5f
                   + r3 * 0.33333333f
                   - r4 * 0.25f
                   + r5 * 0.2f;

   const float LN2 = 0.69314718056f;

   return log_m + exponent * LN2;
}
