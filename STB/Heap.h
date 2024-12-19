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

#pragma once

#include <cstddef>
#include <algorithm>
#include <new>

#include "STB/List.h"

namespace STB {

template <typename TYPE>
using HeapElem = typename STB::List<TYPE>::Elem;

template <typename TYPE, size_t SIZE>
class Heap
{
public:
   Heap()
   {
      reset();
   }

   //! Return pointer to raw allocation
   TYPE* data() { return reinterpret_cast<TYPE*>(heap); }

   //! Return pointer to raw allocation
   const TYPE* data() const { return reinterpret_cast<const TYPE*>(heap); }

   //! Check if heap is empty
   bool empty() const { return free_list.empty(); }

   //! Get number of objects that have been allocated
   size_t allocated() const { return SIZE - free_list.size(); }

   //! Get number of objects that could be allocated
   size_t avail() const { return free_list.size(); }

   //! Return the heap index for an element
   size_t index(const TYPE* object_) const
   {
      return reinterpret_cast<const Proxy*>(object_) - heap;
   }

   //! Reset the heap to nothing allocated
   //  destructors are not called on any active allocations
   void reset()
   {
      free_list.clear();

      for(size_t i = SIZE; i > 0; --i)
      {
         free_list.push_front(&heap[i - 1]);
      }
   }

   //! Allocate a new object
   template<class... ARGS>
   TYPE* alloc(ARGS&&... args)
   {
      Proxy* proxy = free_list.front();

      if (proxy == nullptr)
         return nullptr;

      free_list.pop_front();

      return new (proxy) TYPE(std::forward<ARGS>(args)...);
   }

   //! De-allocate an object
   void free(TYPE* object_)
   {
      object_->~TYPE();

      free_list.push_front(reinterpret_cast<Proxy*>(object_));
   }

private:
   static constexpr size_t MIN_SIZE = std::max(sizeof(uintptr_t), sizeof(TYPE));

   //! Linked list type that takes the same space (or more) as TYPE
   struct Proxy : public STB::List<Proxy>::Elem
   {
      uint8_t pack[MIN_SIZE - sizeof(typename STB::List<Proxy>::Elem)];
   };

   List<Proxy> free_list;
   Proxy       heap[SIZE];
};

} // namespace STB
