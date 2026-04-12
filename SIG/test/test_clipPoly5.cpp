//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/clip/Poly5.h"

#include "STB/Test.h"
#include "round.h"

using namespace SIG;

TEST(SIG_clip_poly5, usage)
{
   clip::Poly5 clip{};

   EXPECT_EQ(-1.0,   clip(-2.0));
   EXPECT_EQ(-0.687, round3(clip(-0.5)));
   EXPECT_EQ( 0.0,   clip( 0.0));
   EXPECT_EQ( 0.688, round3(clip( 0.5)));
   EXPECT_EQ( 1.0,   clip( 2.0));
}

TEST(SIG_clip_poly5, set_n)
{
   clip::Poly5 clip{};

   clip.setN(5.0);

   EXPECT_EQ(0.969, round3(clip(0.5)));
}
