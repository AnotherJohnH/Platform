//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace clip {

//! Poly clip signal to -1.0...+1.0
class Poly
{
public:
   inline Signal operator()(Signal x)
   {
           if (x > Signal(+1.0)) return Signal(+1.0);
      else if (x < Signal(-1.0)) return Signal(-1.0);

      return Signal(1.5) * x - Signal(0.5) * (x * x * x);
   }
};

} // namespace clip

} // namespace SIG
