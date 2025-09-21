//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <array>

#include "SND/SND.h"

namespace SND {

//! General N zero filter
template <unsigned N>
class NPoleFilter : public UnaryOp
{
public:
   std::array<ControlIn<Signal>, N> a;
   ControlIn<Signal> b0;

   NPoleFilter() {}

   NPoleFilter(const std::array<Signal, N>& a_, Signal b0_)
      : a(a_)
      , b0(b0_)
   {}

private:
   double y[N + 1] = {0.0};

   Signal output()
   {
      y[0] = b0 * in;

      for(unsigned i = N; i > 0; --i)
      {
         y[0] -= a[i - 1] * y[i];
         y[i] = y[i - 1];
      }

      return y[0];
   }
};

} // namespace SND

