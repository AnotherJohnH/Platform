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

   EXPECT_EQ(0.0f, delay());

   EXPECT_EQ(0.0f, delay(1.0f));
   EXPECT_EQ(0.0f, delay(2.0f));
   EXPECT_EQ(0.0f, delay(3.0f));

   EXPECT_EQ(1.0f, delay(4.0f));
   EXPECT_EQ(2.0f, delay(5.0f));
   EXPECT_EQ(3.0f, delay(6.0f));

   delay = 7.0f;
   delay = 8.0f;
   delay = 9.0f;

   EXPECT_EQ(7.0f, delay());
   EXPECT_EQ(7.0f, delay(10.0f));
   EXPECT_EQ(8.0f, delay(11.0f));
   EXPECT_EQ(9.0f, delay(12.0f));

    delay.reset();
    EXPECT_EQ(0.0f, delay());
    EXPECT_EQ(0.0f, delay(13.0f));
    EXPECT_EQ(0.0f, delay(14.0f));
    EXPECT_EQ(0.0f, delay(15.0f));
    EXPECT_EQ(13.0f, delay(16.0f));
}
