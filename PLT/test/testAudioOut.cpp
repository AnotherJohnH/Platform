//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "PLT/Audio.h"
#include "PLT/Event.h"

#include "STB/Waveform.h"


class TestAudio : public PLT::Audio::Out
{
private:
   static const unsigned           SAMPLE_FREQUENCY = 44100;
   static const PLT::Audio::Format FORMAT           = PLT::Audio::Format::SINT16;
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
