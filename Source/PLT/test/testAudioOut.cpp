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


#include "PLT/Audio.h"
#include "PLT/Event.h"

#include "STB/Waveform.h"


class TestAudio : public PLT::Audio::Out
{
private:
   static const unsigned           SAMPLE_FREQUENCY = 44100;
   static const PLT::Audio::Format FORMAT           = PLT::Audio::SINT16;
   static const unsigned           CHANNELS         = 1;

   double phase_delta;
   double phase{0.0};

   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         buffer[i] = STB::Waveform::sine(phase) * 0x7FFF;
         phase += phase_delta;
      }
   }

public:
   TestAudio(double freq = 440.0)
      : PLT::Audio::Out(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
      , phase_delta(freq/SAMPLE_FREQUENCY)
   {}
};


int main()
{
   TestAudio audio;

   audio.setEnable(true);

   return PLT::Event::mainLoop();
}
