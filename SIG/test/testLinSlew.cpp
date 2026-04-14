//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/LinSlew.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_LinSlew, steps_linearly_to_target)
{
   LinSlew slew{SAMPLE_RATE / 4};

   slew.set(0.0);
   EXPECT_NEAR(0.0, slew(), 0.0001);

   slew = Signal{1.0};

   EXPECT_NEAR(0.25, slew(), 0.0001);
   EXPECT_NEAR(0.50, slew(), 0.0001);
   EXPECT_NEAR(0.75, slew(), 0.0001);
   EXPECT_NEAR(1.00, slew(), 0.0001);
}

TEST(SIG_LinSlew, set_bypasses_slew)
{
   LinSlew slew{};

   slew.set(2.0);

   EXPECT_NEAR(2.0, slew(), 0.0001);
   EXPECT_NEAR(6.0, slew(3.0), 0.0001);
}
