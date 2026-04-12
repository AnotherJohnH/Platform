//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/LinSlew.h"

#include "STB/Test.h"
#include "Round.h"

using namespace SIG;

TEST(SIG_LinSlew, steps_linearly_to_target)
{
   LinSlew slew{SAMPLE_RATE / 4};

   slew.set(0.0);
   slew = Signal{1.0};

   EXPECT_EQ(round2(0.25), round2(slew()));
   EXPECT_EQ(round2(0.50), round2(slew()));
   EXPECT_EQ(round2(0.75), round2(slew()));
   EXPECT_EQ(round2(1.00), round2(slew()));
}

TEST(SIG_LinSlew, set_bypasses_slew)
{
   LinSlew slew{};

   slew.set(2.0);

   EXPECT_EQ(round2(2.0), round2(slew()));
   EXPECT_EQ(round2(6.0), round2(slew(3.0)));
}
