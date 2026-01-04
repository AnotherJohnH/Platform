//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

class Sine : public Base
{
public:
   Sine() = default;

   Signal operator()()
   {
      float theta = uphase2signal(phase) * M_PI;

      phase += delta;

      return gain(sinf(theta));
   }

   Signal operator()(Signal mod_)
   {
      float theta = uphase2signal(phase) * M_PI;

      phase += modDelta(mod_);

      return gain(sinf(theta));
   }

   Gain gain{};
};

} // namespace Osc

} // namespace SIG
