//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/LogPot.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_LogPot, usage)
{
   LogPot pot{10.0, 0.1};

   EXPECT_NEAR(0.0,  pot(0.0), 0.0001);
   EXPECT_NEAR(0.05, pot(2.5), 0.0001);
   EXPECT_NEAR(0.1,  pot(5.0), 0.0001);   // Half-way point
   EXPECT_NEAR(0.55, pot(7.5), 0.0001);
   EXPECT_NEAR(1.0,  pot(10.0), 0.0001);
}

TEST(SIG_LogPot, re_configure)
{
   LogPot pot{10.0, 0.1};

   pot.config(4.0, 0.25);

   EXPECT_EQ(0.0,   pot(0.0));
   EXPECT_EQ(0.125, pot(1.0));
   EXPECT_EQ(0.25,  pot(2.0));   // Half-way point
   EXPECT_EQ(0.625, pot(3.0));
   EXPECT_EQ(1.0,   pot(4.0));
}
