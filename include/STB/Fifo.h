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

#ifndef STB_FIFO_H
#define STB_FIFO_H

#include <cassert>
#include <cstddef>

namespace STB {

//! Small, portable, statically sized queue
//  Note, when full, one element is wasted
template <typename T, std::size_t LOG2_N, typename INDEX=std::size_t>
class Fifo
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

   //! Get writable reference to newest element
   reference front() { return buffer[prevIndex(write)]; }

   //! Get read-only reference to newest element
   const_reference front() const { return buffer[prevIndex(write)]; }

   //! Get writable reference to oldest element
   reference back() { return buffer[read]; }

   //! Get read-only reference to oldest element
   const_reference back() const { return buffer[read]; }


//------------------------------------------------------------------
// Capacity

   //! Returns true if the FIFO is empty
   bool empty() const { return read == write; }

   //! Returns current number of elements in the FIFO
   size_type size() const { return write >= read ? write - read
                                                 : write - read + N; }

   //! Returns true if the FIFO is full
   bool full() const { return nextIndex(write) == read; }

   //! Returns maximum number of elements that can be in the FIFO
   size_type max_size() const { return N - 1; }


//------------------------------------------------------------------
// Modifiers

   //! Push new element into the FIFO
   void push(const value_type& value)
   {
      if (full())
      {
         // TODO throw something or other
         assert(!"Fifo is full");
         return;
      }

      buffer[write] = value;

      write = nextIndex(write);
   }

   //! Remove oldest element from the FIFO
   void pop()
   {
      if (empty())
      {
         // TODO throw something or other
         assert(!"Fifo is empty");
         return;
      }

      read = nextIndex(read);
   }

   //! Remove all elements
   void clear()
   {
       read = write;
   }

//------------------------------------------------------------------

private:
   static const INDEX N    = INDEX(1)<<LOG2_N;
   static const INDEX MASK = N - 1;

   T buffer[N];

   volatile INDEX   read  = 0;
   volatile INDEX   write = 0;

   static INDEX nextIndex(INDEX index)
   {
       return (index + 1) & MASK;
   }

   static INDEX prevIndex(INDEX index)
   {
       return (index - 1) & MASK;
   }
};

} // namespace STB

#endif
