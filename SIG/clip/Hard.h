//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SIG/Types.h"

namespace SIG {

namespace clip {

//! Hard clip signal to -1.0...+1.0
class Hard
{
public:
   inline Signal operator()(Signal x_)
   {
           if (x_ > Signal(+1.0)) return Signal(+1.0);
      else if (x_ < Signal(-1.0)) return Signal(-1.0);

      return x_;
   }
};

} // namespace clip

} // namespace SIG
