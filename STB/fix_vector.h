//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

