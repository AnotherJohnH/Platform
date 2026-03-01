//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Logarithmic pots aren't...
// + To compensate for human hearing they need to be exponential not logarithmic
// + Non-linear tracks are difficult to make so they are usually approximated with
//   two linear sections

#pragma once

#include "SIG.h"

namespace SIG {

class LogPot
{
public:
   LogPot(SIG::Float max_x, SIG::Float mid_y)
   {
      config(max_x, mid_y);
   }

   void config(SIG::Float max_x, SIG::Float mid_y)
   {
      bp = max_x * 0.5f;
      m1 = mid_y / bp;
      m2 = (1.0f - mid_y) / bp;
      k2 = 1.0f - m2 * max_x;
   }

   Signal operator()(Signal x) const
   {
      return x < bp ? x * m1
                    : x * m2 + k2;
   }

private:
   SIG::Float bp{0.5f};
   SIG::Float m1{1.0f};
   SIG::Float m2{1.0f};
   SIG::Float k2{0.0f};
};

} // namespace SIG
