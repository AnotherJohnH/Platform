//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/osc/Pulse.h"
#include "SIG/osc/Square.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_osc, pulse)
{
   osc::Pulse  osc{};
   osc::Square ref{};

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

      // printf("%7.4f   %7.4f   %7.4f\n", value, ref_value, value - ref_value);
   }
}
