//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/clip/Tanh.h"

#include "STB/Test.h"
#include "round.h"

using namespace SIG;

TEST(SIG_clip_tanh, usage)
{
   clip::Tanh clip{};

   EXPECT_EQ( 0.0,   clip(0.0));
   EXPECT_EQ(-0.904, round3(clip(-0.5)));
   EXPECT_EQ( 0.905, round3(clip( 0.5)));
   EXPECT_EQ( 0.462, round3(clip( 0.5, 1.0)));
}
