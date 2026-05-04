//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <math.h>
#include <stdint.h>

float expf(float x_)
{
   // Clamp to avoid overflow
   if (x_ > +88.0f) return 1.651636e38f;
   if (x_ < -88.0f) return 0.0f;

   // Range reduction
   const float LN2     = 0.69314718056f;
   const float INV_LN2 = 1.44269504089f;

   signed n = signed(x_ * INV_LN2);
   float  r = x_ - n * LN2;

   // 5th order Taylor series to approximate e^x
   float r2 = r * r;
   float r3 = r2 * r;
   float r4 = r3 * r;
   float r5 = r4 * r;

   float exp_r = 1.0f + r + r2 * 0.5f + r3 * 0.16666667f + r4 * 0.04166667f + r5 * 0.008333333f;

   // Build 2^n via float exponent
   union { uint32_t i; float f; } u;
   u.i = uint32_t(n + 127) << 23;

   return exp_r * u.f;
}
