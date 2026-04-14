//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/ExpSlew.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_ExpSlew, converges_to_target)
{
   ExpSlew slew{0.01};

   // Slew from 0.0 to 1.0
   slew.set(0.0);
   slew = Signal{1.0};

   Signal v1 = slew();
   Signal v2 = slew();
   Signal v3 = slew();

   EXPECT_LT(Signal{0.0}, v1);
   EXPECT_GT(Signal{1.0}, v1);
   EXPECT_LT(v1, v2);
   EXPECT_LT(v2, v3);

   for(unsigned i = 0; i < 5000; ++i)
      v3 = slew();

   EXPECT_LT(v2, v3);
   EXPECT_GT(Signal{1.0}, v3);
}

TEST(SIG_ExpSlew, set_bypasses_slew)
{
   ExpSlew slew{0.01};

   slew.set(2.0);

   EXPECT_NEAR(2.0, slew(), 0.001);
   EXPECT_NEAR(Signal{6}, slew(3.0), 0.001);
}
