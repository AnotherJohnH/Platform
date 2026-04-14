//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/Conv.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_Conv, dBGainLookup_15)
{
   EXPECT_NEAR(10.000, dBGainLookup_15(+0x4000), 0.003);
   EXPECT_NEAR(10.000, dBGainLookup_15(+0x2001), 0.003);
   EXPECT_NEAR(10.000, dBGainLookup_15(+0x2000), 0.003);
   EXPECT_NEAR( 1.000, dBGainLookup_15( 0x0000), 0.001);
   EXPECT_NEAR( 0.100, dBGainLookup_15(-0x2000), 0.001);
   EXPECT_NEAR( 0.010, dBGainLookup_15(-0x4000), 0.001);
   EXPECT_NEAR( 0.001, dBGainLookup_15(-0x5FFF), 0.001);
   EXPECT_EQ(   0.0,   dBGainLookup_15(-0x6000));
   EXPECT_EQ(   0.0,   dBGainLookup_15(-0x8000));
}

TEST(SIG_Conv, dBGainLookup)
{
   EXPECT_NEAR(10.000, dBGainLookup(+40.0), 0.003);
   EXPECT_NEAR(10.000, dBGainLookup(+21.0), 0.003);
   EXPECT_NEAR(10.000, dBGainLookup(+20.0), 0.003);
   EXPECT_NEAR( 1.000, dBGainLookup(  0.0), 0.001);
   EXPECT_NEAR( 0.100, dBGainLookup(-20.0), 0.001);
   EXPECT_NEAR( 0.010, dBGainLookup(-40.0), 0.001);
   EXPECT_NEAR( 0.001, dBGainLookup(-60.0), 0.001);
   EXPECT_EQ(   0.0,   dBGainLookup(-61.0));
   EXPECT_EQ(   0.0,   dBGainLookup(-80.0));
}

TEST(SIG_Conv, dBAttenLookup_7)
{
   EXPECT_NEAR(1.000, dBAttenLookup_7(0x00), 0.001);
   EXPECT_NEAR(0.001, dBAttenLookup_7(0x7E), 0.001);
   EXPECT_EQ(  0.0,   dBAttenLookup_7(0x7F));
}
