//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>

//! A vector container with static allocation
template <typename TYPE, size_t N>
class fix_vector
{
public:
   using iterator       = TYPE*;
   using const_iterator = const TYPE*;

   fix_vector() = default;

   fix_vector(size_t n_)
   {
      resize(n_);
   }

   // Capacity
   size_t size()     const { return n; }
   size_t max_size() const { return N; }
   bool   empty()    const { return n == 0; }

   // Element access
   TYPE*       data()       { return reinterpret_cast<TYPE*>(storage); }
   const TYPE* data() const { return reinterpret_cast<const TYPE*>(storage); }

   TYPE&       operator[](size_t index)       { return element(index); }
   const TYPE& operator[](size_t index) const { return element(index); }

   TYPE&       front()       { return element(0); }
   const TYPE& front() const { return element(0); }

   TYPE&       back()       { return element(n - 1); }
   const TYPE& back() const { return element(n - 1); }

   // Iterators
   iterator       begin()        { return &element(0); }
   const_iterator cbegin() const { return &element(0); }

   iterator       end()        { return &element(n); }
   const_iterator cend() const { return &element(n); }

   iterator       rbegin()        { return &element(n - 1); }
   const_iterator crbegin() const { return &element(n - 1); }

   iterator       rend()         { return &element(-1); }
   const_iterator crend() const  { return &element(-1); }

   // Modifiers
   void clear() { n = 0; }

   void reserve(size_t n_) {}

   void resize(size_t n_)
   {
      assert(n_ <= N);
      n = n_;
   }

   void push_back(const TYPE& value_)
   {
      assert(n < N);
      element(n++) = value_;
   }

   void push_back(TYPE&& value_)
   {
      assert(n < N);
      element(n++) = value_;
   }

private:
   TYPE&       element(size_t index)       { return data()[index]; }
   const TYPE& element(size_t index) const { return data()[index]; }

   size_t n{0};
   char   storage[N * sizeof(TYPE)];
};

