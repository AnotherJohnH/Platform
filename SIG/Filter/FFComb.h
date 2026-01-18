//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Delay.h"
#include "SIG/Gain.h"

namespace SIG {

class FFComb
{
public:
   FFComb() = default;

   Signal operator()(Signal x_)
   {
      return x_ + alpha(delay(x_));
   }

   Gain alpha{};

private:
   Delay delay;
};

} // namespace SIG
