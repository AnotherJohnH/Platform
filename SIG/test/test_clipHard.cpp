//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/clip/Hard.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_clip_hard, usage)
{
   clip::Hard clip{};

   EXPECT_EQ(-1.0, clip(-2.0));
   EXPECT_EQ(-0.5, clip(-0.5));
   EXPECT_EQ( 0.0, clip( 0.0));
   EXPECT_EQ( 0.5, clip( 0.5));
   EXPECT_EQ( 1.0, clip( 2.0));
}
