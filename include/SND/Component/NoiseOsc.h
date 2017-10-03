//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#ifndef SND_NOISE_OSC_H
#define SND_NOISE_OSC_H

#include "Osc.h"

namespace SND {

class NoiseOsc : public Osc
{
public:
   NoiseOsc(Freq freq_hz = 0.0) : Osc(freq_hz) {}

private:
   virtual Signal output() override
   {
      unsigned next_phase = nextPhase();

      if (next_phase != last_phase)
      {
         last_phase = next_phase;

         // use xorshift, it's fast and simple
         rand_state ^= rand_state << 13;
         rand_state ^= rand_state >> 17;
         rand_state ^= rand_state <<  5;

         sample = Signal(rand_state >> 8)/(1<<23) - 1.0;
      }

      return sample;
   }

   uint32_t  rand_state{1};
   unsigned  last_phase{0};
   Signal    sample{0};
};

} // namespace SND

#endif
