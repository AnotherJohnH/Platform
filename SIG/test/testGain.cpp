//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/Gain.h"

#include "STB/Test.h"
#include "Round.h"

using namespace SIG;

TEST(SIG_Gain, usage)
{
   Gain gain{};

   EXPECT_EQ(round2(1.0), round2(Signal(gain)));
   EXPECT_EQ(round2(2.0), round2(gain(2.0)));

   gain = 0.5;

   EXPECT_EQ(round2(0.5),  round2(Signal(gain)));
   EXPECT_EQ(round2(1.0),  round2(gain(2.0)));
   EXPECT_EQ(round2(-1.5), round2(gain(-3.0)));
}
