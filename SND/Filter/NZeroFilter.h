//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SND/SND.h"

namespace SND {

//! General N zero filter
template <unsigned N>
class NZeroFilter : public UnaryOp
{
public:
   ControlIn<Signal> b[N + 1];

   NZeroFilter(Signal b_[N + 1])
      : b(b_)
   {}

private:
   double x[N + 1] = {0.0};

   Signal output()
   {
      Signal y0 = 0.0;

      for(int i = N; i >= 0; --i)
      {
         x[i] = i == 0 ? in : x[i - 1];

         y0 += b[i] * x[i];
      }

      return y0;
   }
};

} // namespace SND

