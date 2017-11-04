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

#ifndef SND_WAVE_TABLE_OSC_H
#define SND_WAVE_TABLE_OSC_H

#include "MTH/Waveform.h"
#include "Osc.h"


namespace SND {

//! Base class for table based oscillators
class WaveTableOsc : public Osc
{
protected:
   Signal table[PHASE_PERIOD];

   WaveTableOsc(Freq freq_hz)
      : Osc(freq_hz)
   {}

   WaveTableOsc(Freq freq_hz, double (*func)(double))
      : Osc(freq_hz)
   {
      computeWave(func);
   }

   void computeWave(double (*func)(double))
   {
      Signal local[PHASE_PERIOD];

      double min = +1.0;
      double max = -1.0;

      for(unsigned i = 0; i < PHASE_PERIOD; ++i)
      {
         double value = func(double(i) / PHASE_PERIOD);

         if(value > max) max = value;
         if(value < min) min = value;

         local[i] = value;
      }

      // Rescale to -1..+1
      for(unsigned i = 0; i < PHASE_PERIOD; ++i)
      {
         local[i] = (2.0 * (local[i] - min) / (max - min)) - 1.0;
      }

      // Final copy to live wave table
      for(unsigned i = 0; i < PHASE_PERIOD; ++i)
      {
         table[i] = local[i];
      }
   }

private:
   virtual Signal output() override
   {
      return table[nextPhase()];
   }
};


class SquareOsc : public WaveTableOsc
{
public:
   SquareOsc(Freq freq_hz = 0.0)
      : WaveTableOsc(freq_hz, MTH::Waveform::square)
   {}
};


class SawToothOsc : public WaveTableOsc
{
public:
   SawToothOsc(Freq freq_hz = 0.0)
      : WaveTableOsc(freq_hz, MTH::Waveform::sawTooth)
   {}
};


class TriangleOsc : public WaveTableOsc
{
public:
   TriangleOsc(Freq freq_hz = 0.0)
      : WaveTableOsc(freq_hz, MTH::Waveform::triangle)
   {}
};


class SineOsc : public WaveTableOsc
{
public:
   SineOsc(Freq freq_hz = 0.0)
      : WaveTableOsc(freq_hz, MTH::Waveform::sine)
   {}
};


class CosineOsc : public WaveTableOsc
{
public:
   CosineOsc(Freq freq_hz = 0.0)
      : WaveTableOsc(freq_hz, MTH::Waveform::cosine)
   {}
};


} // namespace SND

#endif
