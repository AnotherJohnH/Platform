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

   EXPECT_EQ(0.0, delay());

   EXPECT_EQ(0.0, delay(1.0));
   EXPECT_EQ(1.0, delay());

   delay = 2.0;
   EXPECT_EQ(2.0, delay());

   EXPECT_EQ(2.0, delay(3.0));
   EXPECT_EQ(3.0, delay());

   delay.reset();
   EXPECT_EQ(0.0, delay());
   EXPECT_EQ(0.0, delay(4.0));
   EXPECT_EQ(4.0, delay());
}
