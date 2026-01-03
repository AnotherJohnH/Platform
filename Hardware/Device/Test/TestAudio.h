//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

const unsigned SAMPLE_RATE      = 48000;
const unsigned FREQ             = 440;
const uint32_t PHASE_INC        = ((FREQ << 22) / SAMPLE_RATE) << 10;
const unsigned FRAC_BITS        = 32 - LOG2_TABLE_SINE_SIZE;
const unsigned SAMPLES_PER_TICK = 1000;

static int16_t oscLeft()
{
   static uint32_t phase{0};

   int16_t sample = table_sine[phase >> FRAC_BITS];
   phase += PHASE_INC;
   return sample;
}

static int16_t oscRight()
{
   static uint32_t phase{0};

   int16_t sample = table_sine[phase >> FRAC_BITS];
   phase += PHASE_INC * 2;
   return sample;
}

static hw::Audio<SAMPLES_PER_TICK>* audio_ptr{nullptr};

extern "C" void IRQ_DMA_0()
{
   if (audio_ptr != nullptr)
   {
      audio_ptr->irqHandler();
   }
}

#if defined(HW_DAC_I2S)

void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   for(unsigned i = 0; i < n; i++)
   {
      buffer[i] = (oscLeft() << 16) | (oscRight() & 0xFFFF);
   }
}

#elif defined(HW_DAC_PWM)

void MTL::Audio::getSamples(uint32_t* buffer, unsigned n)
{
   for(unsigned i = 0; i < n; i++)
   {
      buffer[i] = audio_ptr->packSamples(oscLeft(), oscRight());
   }
}

#else

template<>
void hw::Audio<SAMPLES_PER_TICK>::getSamples(int16_t* buffer, unsigned n)
{
   for(unsigned i = 0; i < n; i += 2)
   {
      buffer[i]     = oscLeft();
      buffer[i + 1] = oscRight();
   }
}

#endif

namespace hw {

inline NOINLINE void testAudio(TestPhase phase_)
{
   static hw::Audio<SAMPLES_PER_TICK> audio{SAMPLE_RATE};

   switch(phase_)
   {
   case DECL:
      audio_ptr = &audio;
      break;

   case INFO:
      printf("AUDIO: Left %u Hz sine wave, Rigth %u Hz sine wave\n", FREQ, FREQ * 2);
      printf("AUDIO: Sample rate = %u Hz\n", SAMPLE_RATE);
      break;

   case START:
      audio.start();
      break;

   case RUN:
      break;
   }
}

} // namespace hw
