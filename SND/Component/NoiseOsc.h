//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_NOISE_OSC_H
#define SND_NOISE_OSC_H

#include "Osc.h"

namespace SND {

class NoiseOsc : public Osc
{
public:
   NoiseOsc(Freq freq_hz = 0.0)
      : Osc(freq_hz)
   {}

private:
   virtual Signal output() override
   {
      unsigned next_phase = nextPhase();

      if(next_phase != last_phase)
      {
         last_phase = next_phase;

         // use xorshift, it's fast and simple
         rand_state ^= rand_state << 13;
         rand_state ^= rand_state >> 17;
         rand_state ^= rand_state << 5;

         sample = Signal(rand_state >> 8) / (1 << 23) - 1.0;
      }

      return sample;
   }

   uint32_t rand_state{1};
   unsigned last_phase{0};
   Signal   sample{0};
};

} // namespace SND

#endif
