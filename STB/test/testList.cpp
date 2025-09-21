//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/List.h"

#include "STB/Test.h"

struct Object : public STB::List<Object>::Elem
{
   unsigned value{0};
};

TEST(STB_List, basic)
{
   STB::List<Object> list;
   Object            heap[4];

   EXPECT_TRUE(list.empty());
   EXPECT_EQ(nullptr, list.front());
   EXPECT_EQ(nullptr, list.back());
   EXPECT_EQ(0, list.size());

   list.push_back(&heap[0]);

   EXPECT_FALSE(list.empty());
   EXPECT_EQ(&heap[0], list.front());
   EXPECT_EQ(&heap[0], list.back());
   EXPECT_EQ(1, list.size());

   list.push_back(&heap[1]);

   EXPECT_FALSE(list.empty());
   EXPECT_EQ(&heap[0], list.front());
   EXPECT_EQ(&heap[1], list.back());
   EXPECT_EQ(2, list.size());

   list.push_front(&heap[2]);

   EXPECT_FALSE(list.empty());
   EXPECT_EQ(&heap[2], list.front());
   EXPECT_EQ(&heap[1], list.back());
   EXPECT_EQ(3, list.size());

   list.pop_front();

   EXPECT_FALSE(list.empty());
   EXPECT_EQ(&heap[0], list.front());
   EXPECT_EQ(&heap[1], list.back());
   EXPECT_EQ(2, list.size());

   list.pop_back();

   EXPECT_FALSE(list.empty());
   EXPECT_EQ(&heap[0], list.front());
   EXPECT_EQ(&heap[0], list.back());
   EXPECT_EQ(1, list.size());

   list.clear();

   EXPECT_TRUE(list.empty());
   EXPECT_EQ(nullptr, list.front());
   EXPECT_EQ(nullptr, list.back());
   EXPECT_EQ(0, list.size());
}

TEST(STB_List, iteration)
{
   const unsigned N = 4;

   STB::List<Object> list;
   Object            heap[N];
   unsigned          factorial{1};

   for(unsigned i = 0; i < N; ++i)
   {
      Object* object = &heap[i];
      object->value  = i + 1;
      list.push_back(object);

      factorial = factorial * object->value;
   }

   unsigned value{1};
   for(const Object* object = list.front(); object; object = object->nextElem())
   {
      value = value * object->value;
   }
   EXPECT_EQ(value, factorial);

   value = 1;
   for(auto it = list.begin(); it != list.end(); ++it)
   {
      value = value * (*it).value;
   }
   EXPECT_EQ(value, factorial);

   value = 1;
   for(const auto& o : list)
   {
      value = value * o.value;
   }
   EXPECT_EQ(value, factorial);
}

TEST(STB_List, insert)
{
   const unsigned N = 7;

   STB::List<Object> list;
   Object            heap[N];

   for(unsigned i = 0; i < N; ++i)
      heap[i].value = i;

   list.push_back(heap + 1);
   list.push_back(heap + 3);
   list.push_back(heap + 5);

   list.insert_after(nullptr,  heap + 0);
   list.insert_after(heap + 1, heap + 2);
   list.insert_after(heap + 3, heap + 4);
   list.insert_after(heap + 5, heap + 6);

   EXPECT_EQ(N, list.size());

   Object* object = list.front();

   for(unsigned i = 0; i < N; ++i)
   {
      EXPECT_EQ(i, object->value);

      object = object->nextElem();
   }
}
