//------------------------------------------------------------------------------
// Copyright (c) 2011 John D. Haughton
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

#ifndef STB_STACK_H
#define STB_STACK_H

#include <cassert>
#include <cstddef>

namespace STB {

//! Small, portable, statically sized stack
template <typename T, std::size_t N, typename INDEX=std::size_t>
class Stack
{
public:

//------------------------------------------------------------------
// Member types

   typedef T            value_type;
   typedef T&           reference;
   typedef const T&     const_reference;
   typedef std::size_t  size_type;

//------------------------------------------------------------------
// Element access

   //! Get writable reference to the oldest element
   reference       front()       { assert(!empty()); return buffer[0]; }

   //! Get read-only reference to oldest element
   const_reference front() const { assert(!empty()); return buffer[0]; }

   //! Get writable reference to the newest element
   reference       back()        { assert(!empty()); return buffer[n - 1]; }

   //! Get read-only reference to newest element
   const_reference back()  const { assert(!empty()); return buffer[n - 1]; }

   //! Get writable reference to any element
   reference       operator[](size_type index)       { assert(index < n); return buffer[index]; }

   //! Get read-only reference to any element
   const_reference operator[](size_type index) const { assert(index < n); return buffer[index]; }

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
      if (full())
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
      if (empty())
      {
         // TODO throw something or other
         assert(!"Stack is empty");
         return;
      }

      --n;
   }

   //! Remove all elements
   void clear()
   {
      n = 0;
   }

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

#endif
