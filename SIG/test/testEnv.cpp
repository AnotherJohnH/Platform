//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/SIG.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_env, adsr)
{
   Env::Adsr env{};

   env.setAttack_mS(5);
   env.setDecay_mS(10);
   env.setSustain(0x40);
   env.setRelease_mS(15);

   env.on();

   const unsigned TEST_MS_LENGTH     = 5 + 10 + 15 + 15;
   const unsigned TEST_SAMPLE_LENGTH = (SAMPLE_RATE / 1000) * TEST_MS_LENGTH;
   const unsigned TEST_SAMPLE_OFF    = TEST_SAMPLE_LENGTH * 2 / 3;

   for(unsigned i = 0; i < TEST_SAMPLE_LENGTH; ++i)
   {
      if (i == TEST_SAMPLE_OFF)
         env.off();

      printf("%5u: %f\n", i, env());
   }
}
