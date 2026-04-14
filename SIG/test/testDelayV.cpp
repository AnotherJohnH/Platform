//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/DelayV.h"

#include "STB/Test.h"

using namespace SIG;

TEST(SIG_DelayV, default_length)
{
   DelayV<3> delay{};

   EXPECT_EQ(3, delay.size());

   EXPECT_EQ(0.0, delay());

   EXPECT_EQ(0.0, delay(1.0));
   EXPECT_EQ(0.0, delay(2.0));
   EXPECT_EQ(0.0, delay(3.0));

   EXPECT_EQ(1.0, delay(4.0));
   EXPECT_EQ(2.0, delay(5.0));
   EXPECT_EQ(3.0, delay(6.0));
}

TEST(SIG_DelayV, length_change_applies_on_wrap)
{
   DelayV<4> delay{};

   EXPECT_EQ(4, delay.size());

   delay.setLength(2);

   EXPECT_EQ(4, delay.size());

   EXPECT_EQ(0.0, delay(1.0));
   EXPECT_EQ(0.0, delay(2.0));
   EXPECT_EQ(0.0, delay(3.0));
   EXPECT_EQ(0.0, delay(4.0));

   EXPECT_EQ(2, delay.size());

   EXPECT_EQ(1.0, delay());
   EXPECT_EQ(1.0, delay(5.0));
   EXPECT_EQ(2.0, delay(6.0));
   EXPECT_EQ(5.0, delay(7.0));
   EXPECT_EQ(6.0, delay(8.0));
}

TEST(SIG_DelayV, zero_length_clamps_to_one)
{
   DelayV<4> delay{};

   delay.setLength(0);

   EXPECT_EQ(4, delay.size());

   EXPECT_EQ(0.0, delay(1.0));
   EXPECT_EQ(0.0, delay(2.0));
   EXPECT_EQ(0.0, delay(3.0));
   EXPECT_EQ(0.0, delay(4.0));

   EXPECT_EQ(1, delay.size());

   EXPECT_EQ(1.0, delay());
   EXPECT_EQ(1.0, delay(5.0));
   EXPECT_EQ(5.0, delay(6.0));
}

TEST(SIG_DelayV, reset_restores_default_state)
{
   DelayV<4> delay{};

   delay.setLength(2);

   EXPECT_EQ(0.0, delay(1.0));
   EXPECT_EQ(0.0, delay(2.0));
   EXPECT_EQ(0.0, delay(3.0));
   EXPECT_EQ(0.0, delay(4.0));

   EXPECT_EQ(2, delay.size());

   delay.reset();

   EXPECT_EQ(4, delay.size());
   EXPECT_EQ(0.0, delay());
   EXPECT_EQ(0.0, delay(5.0));
   EXPECT_EQ(0.0, delay(6.0));
   EXPECT_EQ(0.0, delay(7.0));
   EXPECT_EQ(0.0, delay(8.0));
   EXPECT_EQ(5.0, delay(9.0));
}
