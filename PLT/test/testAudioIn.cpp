//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdint>

#include "PLT/Audio.h"
#include "PLT/Event.h"


static const unsigned           SAMPLE_FREQUENCY = 44100;
static const PLT::Audio::Format FORMAT           = PLT::Audio::Format::SINT16;
static const unsigned           CHANNELS         = 1;


// TODO this just happens to match the SDL2 implementation
//      need a more portable way of doing this
static const unsigned BLOCK_SAMPLES    = 1024;


static int16_t data[BLOCK_SAMPLES];


class TestAudioIn : public PLT::Audio::In
{
public:
   TestAudioIn()
      : PLT::Audio::In(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
   {}

   virtual void setSamples(const int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         data[i] = buffer[i];
      }
   }
};


class TestAudioOut : public PLT::Audio::Out
{
public:
   TestAudioOut()
      : PLT::Audio::Out(SAMPLE_FREQUENCY, FORMAT, CHANNELS)
   {}

   virtual void getSamples(int16_t* buffer, unsigned n) override
   {
      for(unsigned i = 0; i < n; ++i)
      {
         buffer[i] = data[i];
      }
   }
};


int main()
{
   TestAudioIn  audio_in;
   TestAudioOut audio_out;

   audio_in.setEnable(true);
   audio_out.setEnable(true);

   return PLT::Event::mainLoop();
}
