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

// Stub Sounder implementation using PLT::Audio

#include "PLT/Sounder.h"
#include "PLT/Audio.h"
#include "STB/Waveform.h"


class Beep : public PLT::Audio::Out
{
private:
   static const unsigned           SAMPLE_FREQUENCY = 44100; // 44.1 KHz
   static const PLT::Audio::Format FORMAT           = PLT::Audio::Format::SINT16;
   static const unsigned           CHANNELS         = 1;
   static const unsigned           FREQUENCY        = 440;   // 440 Hz

   double phase{0.0};
   double delta;

   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         buffer[i] = STB::Waveform::sine(phase) * 0x7FFF;
         phase += delta;
      }
   }

public:
   Beep()
      : PLT::Audio::Out(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
      , delta(double(FREQUENCY)/SAMPLE_FREQUENCY)
   {}
};


void PLT::Sounder::setEnable(bool enable)
{
   static Beep beep;

   beep.setEnable(enable);
}
