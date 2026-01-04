//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/SIG.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_osc, square)
{
   Osc::Square osc{};

   for(const auto& freq : {55.0, 110.0, 220.0, 440.0})
   {
      osc.sync();
      osc.setFreq(freq);

      Signal   last{0.0};
      unsigned start{0};

      for(unsigned i = 0; i < 1000; ++i)
      {
         Signal value = osc();

         EXPECT_LE(-1.0f, value);
         EXPECT_GE(+1.0f, value);

         if ((i >= 0) && (i < 5))
         {
            EXPECT_LE(0.0f, value);
         }

         if ((last >= 0.0) && (value < 0.0))
         {
            if (start == 0)
            {
               start = i;
            }
            else
            {
               unsigned samples  = i - start;
               double   freq_est = double(SAMPLE_RATE) / samples;
               double   error    = fabs(freq_est - freq) / freq;
               EXPECT_GT(0.001, error);
               break;
            }
         }

         last = value;
      }
   }
}

TEST(SIG_osc, pulse)
{
   Osc::Pulse  osc{};
   Osc::Square ref{};

   osc.setPattern(uint16_t(0xFF00));

   osc.setFreq(440.0);
   ref.setFreq(440.0);

   osc.sync();
   ref.sync();

   (void)osc();

   for(unsigned i = 0; i < 1000; ++i)
   {
      Signal value     = osc();
      Signal ref_value = ref();

      printf("%7.4f   %7.4f   %7.4f\n", value, ref_value, value - ref_value);
   }
}
