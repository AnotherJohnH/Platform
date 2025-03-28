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

#include "STB/Heap.h"

#include "STB/Test.h"

struct Object
{
   Object(unsigned number_)
     : number(number_)
   {
   }

   unsigned number;
};

TEST(STB_Heap, basic)
{
   STB::Heap<Object,10> heap;

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(0, heap.allocated());

   Object* obj = heap.alloc(5);

   EXPECT_NE(nullptr, obj);
   EXPECT_EQ(5, obj->number);

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(1, heap.allocated());

   heap.free(obj);

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(0, heap.allocated());
}

TEST(STB_Heap, full)
{
   STB::Heap<Object,3> heap;

   Object* one     = heap.alloc(1);
   size_t  one_idx = heap.index(one);

   EXPECT_NE(nullptr, one);
   EXPECT_EQ(1, one->number);
   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(1, heap.allocated());

   Object* two = heap.alloc(2);
   size_t  two_idx = heap.index(two);

   EXPECT_NE(nullptr, two);
   EXPECT_EQ(2, two->number);
   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(2, heap.allocated());

   Object* three     = heap.alloc(3);
   size_t  three_idx = heap.index(three);

   EXPECT_NE(nullptr, three);
   EXPECT_EQ(3, three->number);
   EXPECT_TRUE(heap.empty());
   EXPECT_EQ(3, heap.allocated());

   EXPECT_NE(one_idx, two_idx);
   EXPECT_NE(one_idx, three_idx);
   EXPECT_NE(two_idx, three_idx);

   Object* four = heap.alloc(4);

   EXPECT_EQ(nullptr, four);
   EXPECT_TRUE(heap.empty());
   EXPECT_EQ(3, heap.allocated());

   heap.free(one);

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(2, heap.allocated());

   heap.free(three);

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(1, heap.allocated());

   heap.free(two);

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(0, heap.allocated());
}

TEST(STB_Heap, reset)
{
   STB::Heap<Object,10> heap;

   heap.alloc(23);
   heap.alloc(56);
   heap.alloc(47);
   heap.alloc(68);

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(4, heap.allocated());

   heap.reset();

   EXPECT_FALSE(heap.empty());
   EXPECT_EQ(0, heap.allocated());
}
