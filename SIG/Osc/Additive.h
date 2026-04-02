//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "SIG/Osc/WaveTable.h"

namespace SIG {

namespace Osc {

template <unsigned N>
class Additive : public WaveTable
{
public:
   Additive() = default;

   void compute()
   {
      computeWave(thunk, this);
   }

   Float a[N + 1] = {};
   Float b[N + 1] = {};

private:
   // TODO convert back to Lambda when embedded builds allow
   Signal func(Signal t)
   {
      Signal value = a[0] / 2.0;

      for(size_t n = 1; n <= N; n++)
      {
         value += a[n] * sinf(n * t * float(2 * M_PI));
         value += b[n] * cosf(n * t * float(2 * M_PI));
      }

      return value;
   }

   static Signal thunk(Signal t, void* that)
   {
      Additive* osc = (Additive*)that;
      return osc->func(t);
   }
};

} // namespace Osc

} // namespace SIG
