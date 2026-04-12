//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/LogPot.h"

#include "STB/Test.h"
#include "round.h"

using namespace SIG;

TEST(SIG_LogPot, usage)
{
   LogPot pot{10.0, 0.1};

   EXPECT_EQ(round3(0.0),  round3(pot(0.0)));
   EXPECT_EQ(round3(0.05), round3(pot(2.5)));
   EXPECT_EQ(round3(0.1),  round3(pot(5.0)));   // Half-way point
   EXPECT_EQ(round3(0.55), round3(pot(7.5)));
   EXPECT_EQ(round3(1.0),  round3(pot(10.0)));
}

TEST(SIG_LogPot, re_configure)
{
   LogPot pot{10.0, 0.1};

   pot.config(4.0, 0.25);

   EXPECT_EQ(round3(0.0),   round3(pot(0.0)));
   EXPECT_EQ(round3(0.125), round3(pot(1.0)));
   EXPECT_EQ(round3(0.25),  round3(pot(2.0)));   // Half-way point
   EXPECT_EQ(round3(0.625), round3(pot(3.0)));
   EXPECT_EQ(round3(1.0),   round3(pot(4.0)));
}
