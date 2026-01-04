//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

class Noise
{
public:
   Noise() = default;

   Signal operator()()
   {
      noise_state ^= noise_state << 13;
      noise_state ^= noise_state >> 17;
      noise_state ^= noise_state << 5;

      return gain(Signal(int32_t(noise_state)) / 0x7FFFFFFF);
   }

   Gain gain{};

private:
   uint32_t noise_state{1};
};

} // namespace Osc

} // namespace SIG
