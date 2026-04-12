//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/Conv.h"

#include "STB/Test.h"
#include "round.h"

using namespace SIG;

TEST(SIG_Conv, dBGainLookup_15)
{
   EXPECT_EQ(round2(Signal{10.000}), round2(dBGainLookup_15(+0x4000)));
   EXPECT_EQ(round2(Signal{10.000}), round2(dBGainLookup_15(+0x2001)));
   EXPECT_EQ(round2(Signal{10.000}), round2(dBGainLookup_15(+0x2000)));
   EXPECT_EQ(round2(Signal{ 1.000}), round2(dBGainLookup_15( 0x0000)));
   EXPECT_EQ(round2(Signal{ 0.100}), round2(dBGainLookup_15(-0x2000)));
   EXPECT_EQ(round2(Signal{ 0.010}), round2(dBGainLookup_15(-0x4000)));
   EXPECT_EQ(round2(Signal{ 0.001}), round2(dBGainLookup_15(-0x5FFF)));
   EXPECT_EQ(Signal{0.0}, dBGainLookup_15(-0x6000));
   EXPECT_EQ(Signal{0.0}, dBGainLookup_15(-0x8000));
}

TEST(SIG_Conv, dBGainLookup)
{
   EXPECT_EQ(round2(Signal{10.000}), round2(dBGainLookup(+40.0)));
   EXPECT_EQ(round2(Signal{10.000}), round2(dBGainLookup(+21.0)));
   EXPECT_EQ(round2(Signal{10.000}), round2(dBGainLookup(+20.0)));
   EXPECT_EQ(round2(Signal{ 1.000}), round2(dBGainLookup(  0.0)));
   EXPECT_EQ(round2(Signal{ 0.100}), round2(dBGainLookup(-20.0)));
   EXPECT_EQ(round2(Signal{ 0.010}), round2(dBGainLookup(-40.0)));
   EXPECT_EQ(round2(Signal{ 0.001}), round2(dBGainLookup(-60.0)));
   EXPECT_EQ(Signal{0.0}, dBGainLookup(-61.0));
   EXPECT_EQ(Signal{0.0}, dBGainLookup(-80.0));
}

TEST(SIG_Conv, dBAttenLookup_7)
{
   EXPECT_EQ(round2(Signal{1.000}), round2(dBAttenLookup_7(0x00)));
   EXPECT_EQ(round2(Signal{0.001}), round2(dBAttenLookup_7(0x7E)));
   EXPECT_EQ(Signal{0.0}, dBAttenLookup_7(0x7F));
}
