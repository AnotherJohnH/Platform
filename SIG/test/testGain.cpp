//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/Gain.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_Gain, usage)
{
   Gain gain{};

   EXPECT_NEAR(1.0, Signal(gain), 0.0001);
   EXPECT_NEAR(2.0, gain(2.0), 0.0001);

   gain = 0.5;

   EXPECT_NEAR(0.5,  Signal(gain), 0.0001);
   EXPECT_NEAR(1.0,  gain(2.0), 0.0001);
   EXPECT_NEAR(-1.5, gain(-3.0), 0.0001);
}
