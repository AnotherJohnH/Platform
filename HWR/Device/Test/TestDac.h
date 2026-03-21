//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

const unsigned SAMPLE_RATE = 48000;
const unsigned FREQ        = 440;
const uint32_t PHASE_INC   = ((FREQ << 22) / SAMPLE_RATE) << 10;
const unsigned FRAC_BITS   = 32 - LOG2_TABLE_SINE_SIZE;

static hw::Dac* dac_ptr{nullptr};

#if !defined(HW_NATIVE)
static void runDAC()
{
   while(true)
   {
      uint32_t phase{0};

      dac_ptr->setSampleRate(48000);

      for(unsigned i = 0; i < (SAMPLE_RATE / 2); ++i)
      {
         int16_t sample = table_sine[phase >> FRAC_BITS];

         phase += PHASE_INC;

         dac_ptr->push(sample, sample);
      }

      dac_ptr->setSampleRate(24000);

      for(unsigned i = 0; i < (SAMPLE_RATE / 4); ++i)
      {
         dac_ptr->push(0, 0);
      }
   }
}
#endif

inline NOINLINE void testDac(TestPhase phase_)
{
   static hw::Dac dac{SAMPLE_RATE};

   switch(phase_)
   {
   case DECL:
      dac_ptr = &dac;
      break;

   case INFO:
      printf("DAC: Pulsed %u Hz sine wave, 0.5s on 0.5s off\n", FREQ);
      printf("DAC: Sample rate     = %u Hz\n", SAMPLE_RATE);
      printf("DAC: Phase inc       = %08x\n", PHASE_INC);
      printf("DAC: Phase frac bits = %u\n", FRAC_BITS);
      break;

   case START:
#if !defined(HW_NATIVE)
      MTL_start_core(1, runDAC);
#endif
      break;

   case RUN:
      break;
   }
}

} // namespace hw
