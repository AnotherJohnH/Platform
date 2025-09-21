//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub Sounder implementation using PLT::Audio

#include "PLT/Sounder.h"
#include "PLT/Audio.h"
#include "STB/Waveform.h"


class Beep : public PLT::Audio::Out
{
public:
   Beep()
      : PLT::Audio::Out(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
   {}

   void control(unsigned gain_8, unsigned freq)
   {
      if (gain_8 == 0)
      {
         setEnable(false);
      }
      else
      {
         delta = double(freq) / SAMPLE_FREQUENCY;
         gain  = gain_8 * 0x80;

         setEnable(true);
      }
   }

private:
   static const unsigned           SAMPLE_FREQUENCY = 44100; // 44.1 KHz
   static const PLT::Audio::Format FORMAT           = PLT::Audio::Format::SINT16;
   static const unsigned           CHANNELS         = 1;

   double   phase{0.0};
   double   delta;
   unsigned gain{0};

   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      // XXX isn't phase going to run out of precision?
      for(unsigned i = 0; i < n; ++i)
      {
         buffer[i] = STB::Waveform::sine(phase) * gain;
         phase += delta;
      }
   }
};


void PLT::Sounder::setEnable(bool enable)
{
   control(enable ? 0xFF : 0x00, 440);
}

void PLT::Sounder::control(unsigned gain_8, unsigned freq)
{
   static Beep beep;

   beep.control(gain_8, freq);
}
