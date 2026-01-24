//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/SIG.h"

#include "STB/Test.h"

using namespace SIG;

//! Round to 2 decimal places
constexpr float round2(float value_) { return signed(value_ * 100.0f + 0.5) * 0.01f; }

TEST(SIG_conv, gain)
{
   EXPECT_EQ(round2(10.000f), round2(dBGainLookup_15(+0x4000)));
   EXPECT_EQ(round2(10.000f), round2(dBGainLookup_15(+0x2001)));
   EXPECT_EQ(round2(10.000f), round2(dBGainLookup_15(+0x2000)));
   EXPECT_EQ(round2( 1.000f), round2(dBGainLookup_15( 0x0000)));
   EXPECT_EQ(round2( 0.100f), round2(dBGainLookup_15(-0x2000)));
   EXPECT_EQ(round2( 0.010f), round2(dBGainLookup_15(-0x4000)));
   EXPECT_EQ(round2( 0.001f), round2(dBGainLookup_15(-0x5FFF)));
   EXPECT_EQ(0.0f, dBGainLookup_15(-0x6000));
   EXPECT_EQ(0.0f, dBGainLookup_15(-0x8000));

   EXPECT_EQ(round2(10.000f), round2(dBGainLookup(+40.0)));
   EXPECT_EQ(round2(10.000f), round2(dBGainLookup(+21.0)));
   EXPECT_EQ(round2(10.000f), round2(dBGainLookup(+20.0)));
   EXPECT_EQ(round2( 1.000f), round2(dBGainLookup(  0.0)));
   EXPECT_EQ(round2( 0.100f), round2(dBGainLookup(-20.0)));
   EXPECT_EQ(round2( 0.010f), round2(dBGainLookup(-40.0)));
   EXPECT_EQ(round2( 0.001f), round2(dBGainLookup(-60.0)));
   EXPECT_EQ(0.0f, dBGainLookup(-61.0));
   EXPECT_EQ(0.0f, dBGainLookup(-80.0));

   EXPECT_EQ(round2( 1.000f), round2(dBAttenLookup_7(0x00)));
   EXPECT_EQ(round2( 0.001f), round2(dBAttenLookup_7(0x7E)));
   EXPECT_EQ(0.0f, dBAttenLookup_7(0x7F));
}
