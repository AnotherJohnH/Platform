//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Logarithmic pots aren't...
// + To compensate for human hearing they need to be exponential not logarithmic
// + Non-linear tracks are difficult to make so they are usually approximated with
//   two linear sections

#pragma once

#include "Types.h"

namespace SIG {

class LogPot
{
public:
   LogPot(Float max_x, Float mid_y)
   {
      config(max_x, mid_y);
   }

   void config(Float max_x, Float mid_y)
   {
      bp = max_x * Float{0.5};
      m1 = mid_y / bp;
      m2 = (Float{1.0} - mid_y) / bp;
      k2 = Float{1.0} - m2 * max_x;
   }

   Signal operator()(Signal x) const
   {
      return x < bp ? x * m1
                    : x * m2 + k2;
   }

private:
   Float bp{0.5};
   Float m1{1.0};
   Float m2{1.0};
   Float k2{0.0};
};

} // namespace SIG
