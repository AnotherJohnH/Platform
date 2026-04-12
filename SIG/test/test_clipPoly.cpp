//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/clip/Poly.h"

#include "STB/Test.h"
#include "round.h"

using namespace SIG;

TEST(SIG_clip_poly, usage)
{
   clip::Poly clip{};

   EXPECT_EQ(-1.0,   clip(-2.0));
   EXPECT_EQ(-0.687, round3(clip(-0.5)));
   EXPECT_EQ( 0.0,   clip( 0.0));
   EXPECT_EQ( 0.688, round3(clip( 0.5)));
   EXPECT_EQ( 1.0,   clip( 2.0));
}
