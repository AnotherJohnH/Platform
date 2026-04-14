//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/clip/Tanh.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_clip_tanh, usage)
{
   clip::Tanh clip{};

   EXPECT_EQ(   0.0,   clip(0.0));
   EXPECT_NEAR(-0.905, clip(-0.5), 0.001);
   EXPECT_NEAR( 0.905, clip( 0.5), 0.001);
   EXPECT_NEAR( 0.462, clip( 0.5, 1.0), 0.001);
}
