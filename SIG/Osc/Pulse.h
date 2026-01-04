//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"

namespace SIG {

namespace Osc {

class Pulse : public Base
{
public:
   Pulse() = default;

   template <typename TYPE>
   void setPattern(TYPE pattern_)
   {
      unsigned num_bits        = sizeof(TYPE) * 8;
      unsigned signal = PATTERN_LENGTH / num_bits;

      unsigned index = 0;

      for(unsigned bit = 0; bit < num_bits; ++bit)
      {
         TYPE mask  = TYPE(1) << (num_bits - bit - 1);
         bool value = (pattern_ & mask) != 0;

         for(unsigned j = 0; j < signal; ++j)
         {
            pattern[index++] = value;
         }
      }
   }

   Signal operator()()
   {
      unsigned index = phase >> (sizeof(UPhase) * 8 - LOG2_PATTERN_LENGTH);

      delay[2] = delay[1];
      delay[1] = delay[0];
      delay[0] = pattern[index];

      Signal signal = delay[1] ? +1.0f : -1.0f;

      if ((not delay[0] && delay[1]) || (delay[1] && not delay[2]))
      {
         float t = uphase2float(phase);
         signal += polyBLEP(t);
      }
      else if ((delay[0] && not delay[1]) || (not delay[1] && delay[2]))
      {
         float t = uphase2float(phase);
         signal -= polyBLEP(t);
      }

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      setDelta(modDelta(mod_));

      unsigned index = phase >> (sizeof(UPhase) * 8 - LOG2_PATTERN_LENGTH);

      delay[2] = delay[1];
      delay[1] = delay[0];
      delay[0] = pattern[index];

      Signal signal = delay[1] ? +1.0f : -1.0f;

      if ((not delay[0] && delay[1]) || (delay[1] && not delay[2]))
      {
         float t = uphase2float(phase);
         signal += polyBLEP(t);
      }
      else if ((delay[0] && not delay[1]) || (not delay[1] && delay[2]))
      {
         float t = uphase2float(phase);
         signal -= polyBLEP(t);
      }

      phase += delta;

      return gain(signal);
   }

   Gain gain{};

private:
   static const unsigned LOG2_PATTERN_LENGTH = 5;
   static const uint64_t PATTERN_LENGTH      = 1 << LOG2_PATTERN_LENGTH;

   bool pattern[PATTERN_LENGTH];
   bool delay[3];
};

} // namespace Osc

} // namespace SIG
