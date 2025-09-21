//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/BitArray.h"

#include "STB/Test.h"

TEST(STB_BitArray, basic)
{
   STB::BitArray<128> bits{};

   EXPECT_FALSE(bits[0]);
   EXPECT_FALSE(bits[127]);
   EXPECT_EQ(128, bits.size());
   EXPECT_FALSE(bits.any());
   EXPECT_FALSE(bits.all());
   EXPECT_TRUE(bits.none());

   bits.set(10);
   EXPECT_TRUE(bits[10]);
   EXPECT_TRUE(bits.any());
   EXPECT_FALSE(bits.all());
   EXPECT_FALSE(bits.none());

   bits.reset(10);
   EXPECT_FALSE(bits[10]);
   EXPECT_FALSE(bits.any());
   EXPECT_FALSE(bits.all());
   EXPECT_TRUE(bits.none());

   bits[78] = true;
   EXPECT_TRUE(bits.test(78));

   bits.reset();
   EXPECT_FALSE(bits.any());
   EXPECT_FALSE(bits.all());
   EXPECT_TRUE(bits.none());
}

TEST(STB_BitArray, odd_size)
{
   STB::BitArray<33> bits{};

   EXPECT_FALSE(bits[0]);
   EXPECT_FALSE(bits[32]);
   EXPECT_EQ(33, bits.size());
   EXPECT_FALSE(bits.any());
   EXPECT_FALSE(bits.all());
   EXPECT_TRUE(bits.none());

   bits[32] = true;
   EXPECT_TRUE(bits.any());
   EXPECT_FALSE(bits.all());
   EXPECT_FALSE(bits.none());

   bits.set(0, 31);
   EXPECT_TRUE(bits.any());
   EXPECT_TRUE(bits.all());
   EXPECT_FALSE(bits.none());

   bits.reset(10, 12);
   EXPECT_TRUE(bits[9]);
   EXPECT_FALSE(bits[10]);
   EXPECT_FALSE(bits[11]);
   EXPECT_FALSE(bits[12]);
   EXPECT_TRUE(bits[13]);
}
