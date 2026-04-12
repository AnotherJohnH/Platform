//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/clip/No.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_clip_no, usage)
{
   clip::No clip{};

   EXPECT_EQ(-1.5, clip(-1.5));
   EXPECT_EQ( 0.0, clip( 0.0));
   EXPECT_EQ( 1.5, clip( 1.5));
}
