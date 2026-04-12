//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/Delay.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_Delay, usage)
{
   Delay delay{};

   EXPECT_EQ(1, delay.size());

   EXPECT_EQ(0.0f, delay());

   EXPECT_EQ(0.0f, delay(1.0f));
   EXPECT_EQ(1.0f, delay());

   delay = 2.0f;
   EXPECT_EQ(2.0f, delay());

   EXPECT_EQ(2.0f, delay(3.0f));
   EXPECT_EQ(3.0f, delay());

   delay.reset();
   EXPECT_EQ(0.0f, delay());
   EXPECT_EQ(0.0f, delay(4.0f));
   EXPECT_EQ(4.0f, delay());
}
