//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Delay.h"
#include "SIG/Gain.h"

namespace SIG {

class FBComb
{
public:
   FBComb() = default;

   Signal operator()(Signal x_)
   {
      Signal y = x_ + alpha(delay());

      delay = y;

      return y;
   }

   Gain alpha{};

private:
   Delay delay;
};

} // namespace SIG
