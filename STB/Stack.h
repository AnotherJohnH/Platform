//-------------------------------------------------------------------------------
// Copyright (c) 2011 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <cstddef>

namespace STB {

//! Small, portable, statically sized stack
template <typename T, size_t N, typename INDEX = size_t>
class Stack
{
public:
   //------------------------------------------------------------------
   // Member types

   using value_type      = T;
   using reference       = T&;
   using const_reference = const T&;
   using size_type       = size_t;

   //------------------------------------------------------------------
   // Element access

   //! Get writable reference to the oldest element
   reference front()
   {
      assert(!empty());
      return buffer[0];
   }

   //! Get read-only reference to oldest element
   const_reference front() const
   {
      assert(!empty());
      return buffer[0];
   }

   //! Get writable reference to the newest element
   reference back()
   {
      assert(!empty());
      return buffer[n - 1];
   }

   //! Get read-only reference to newest element
   const_reference back() const
   {
      assert(!empty());
      return buffer[n - 1];
   }

   //! Get writable reference to any element
   reference operator[](size_type index)
   {
      assert(index < n);
      return buffer[index];
   }

   //! Get read-only reference to any element
   const_reference operator[](size_type index) const
   {
      assert(index < n);
      return buffer[index];
   }

   //------------------------------------------------------------------
   // Capacity

   //! Returns true if the stack is empty
   bool empty() const { return n == 0; }

   //! Returns current number of elements in the stack
   size_type size() const { return n; }

   //! Returns maximum number of elements that can be in the stack
   size_type max_size() const { return N; }

   //! Returns true if the stack is full
   bool full() const { return n == N; }

   //------------------------------------------------------------------
   // Modifiers

   //! Push new element into the stack
   void push_back(const value_type& value)
   {
      if(full())
      {
         // TODO throw something or other
         assert(!"Stack is full");
         return;
      }

      buffer[n++] = value;
   }

   //! Remove newest element from the stack
   void pop_back()
   {
      if(empty())
      {
         // TODO throw something or other
         assert(!"Stack is empty");
         return;
      }

      --n;
   }

   //! Remove all elements
   void clear() { n = 0; }

   //! Resize stack
   void resize(size_type n_)
   {
      assert(n_ <= n);
      n = n_;
   }

   //------------------------------------------------------------------

private:
   T buffer[N];

   INDEX n{0};
};

} // namespace STB

