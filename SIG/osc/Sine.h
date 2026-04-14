//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#if defined(SIG_FL32) || defined(SIG_FL64)
#include <cmath>
#endif

#include "Base.h"

namespace SIG::osc {

class Sine : public Base
{
public:
   Sine() = default;

   Signal operator()()
   {
      float theta = uphase2signal(phase) * float(M_PI);

      phase += delta;

#if defined(SIG_FL32)
      return gain(sinf(theta));
#elif defined(SIG_FL64)
      return gain(sin(theta));
#else
      return 0;
#endif
   }

   Signal operator()(Signal mod_)
   {
      float theta = uphase2signal(phase) * float(M_PI);

      phase += modDelta(mod_);

#if defined(SIG_FL32)
      return gain(sinf(theta));
#elif defined(SIG_FL64)
      return gain(sin(theta));
#else
      return 0;
#endif
   }
};

} // namespace SIG::osc
