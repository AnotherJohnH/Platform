//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/DelayN.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_DelayN, usage)
{
   DelayN<3> delay{};

   EXPECT_EQ(3, delay.size());

   EXPECT_EQ(0.0, delay());

   EXPECT_EQ(0.0, delay(1.0));
   EXPECT_EQ(0.0, delay(2.0));
   EXPECT_EQ(0.0, delay(3.0));

   EXPECT_EQ(1.0, delay(4.0));
   EXPECT_EQ(2.0, delay(5.0));
   EXPECT_EQ(3.0, delay(6.0));

   delay = 7.0;
   delay = 8.0;
   delay = 9.0;

   EXPECT_EQ(7.0, delay());
   EXPECT_EQ(7.0, delay(10.0));
   EXPECT_EQ(8.0, delay(11.0));
   EXPECT_EQ(9.0, delay(12.0));

    delay.reset();
    EXPECT_EQ(0.0, delay());
    EXPECT_EQ(0.0, delay(13.0));
    EXPECT_EQ(0.0, delay(14.0));
    EXPECT_EQ(0.0, delay(15.0));
    EXPECT_EQ(13.0, delay(16.0));
}
