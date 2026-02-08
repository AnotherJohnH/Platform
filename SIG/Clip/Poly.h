//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace Clip {

//! Poly clip signal to -1.0...+1.0
class Poly
{
public:
   inline Signal operator()(Signal x)
   {
           if (x > +1.0f) return +1.0f;
      else if (x < -1.0f) return -1.0f;

      return 1.5f * x - 0.5f * (x * x * x);
   }
};

} // namespace Clip

} // namespace SIG
