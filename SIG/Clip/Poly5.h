//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace Clip {

//! Poly clip (order 5) signal to -1.0...+1.0
class Poly5
{
public:
   Poly5()
   {
      setN(0.0f);
   }

   void setN(float n_)
   {
      a = n_ / 5.0f;
      b = - (4.0f * n_ + 5.0f) / 10.0f;
      c = (2.0 * n_ + 15.0f) / 10.0f;
   }

   inline Signal operator()(Signal x)
   {
           if (x > +1.0f) return +1.0f;
      else if (x < -1.0f) return -1.0f;

      float x3 = x * x * x;
      float x5 = x3 * x * x;
      float y  = a * x5 + b * x3 + c * x;

           if (y > +1.0f) return +1.0f;
      else if (y < -1.0f) return -1.0f;

      return y;
   }

private:
   float a{0.0};
   float b{0.0};
   float c{0.0};
};

} // namespace Clip

} // namespace SIG
