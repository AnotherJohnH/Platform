//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

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
