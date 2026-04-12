//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace clip {

//! Poly clip (order 5) signal to -1.0...+1.0
class Poly5
{
public:
   Poly5()
   {
      setN(0.0);
   }

   void setN(Float n_)
   {
      a = n_ / Float(5.0);
      b = - (Float(4.0) * n_ + Float(5.0)) / Float(10.0);
      c = (Float(2.0) * n_ + Float(15.0)) / Float(10.0);
   }

   inline Signal operator()(Signal x)
   {
           if (x > Signal(+1.0)) return Signal(+1.0);
      else if (x < Signal(-1.0)) return Signal(-1.0);

      Float x3 = x * x * x;
      Float x5 = x3 * x * x;
      Float y  = a * x5 + b * x3 + c * x;

           if (y > Signal(+1.0)) return Signal(+1.0);
      else if (y < Signal(-1.0)) return Signal(-1.0);

      return y;
   }

private:
   Float a{0.0};
   Float b{0.0};
   Float c{0.0};
};

} // namespace clip

} // namespace SIG
