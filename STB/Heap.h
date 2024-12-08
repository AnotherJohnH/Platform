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
#include <new>

namespace STB {

template <typename TYPE>
struct HeapElem
{
   TYPE* next{nullptr};
};

template <typename TYPE, size_t SIZE>
class Heap
{
public:
   Heap()
   {
      reset();
   }

   //! Check if heap is empty
   bool empty() const { return free_list == nullptr; }

   //! Get number of objects that have been allocated
   size_t allocated() const { return num_alloc; }

   //! Return the heap index for an element
   size_t index(const TYPE* object_) const { return object_ - heap; }

   //! Reset the heap to nothing allocated
   void reset()
   {
      free_list = nullptr;
      num_alloc = SIZE;

      for(size_t i = 0; i < SIZE; ++i)
      {
         free(&heap[i]);
      }
   }

   //! Allocate a new object
   TYPE* alloc()
   {
      if (empty())
         return nullptr;

      TYPE* object = free_list;
      free_list    = free_list->next;

      ++num_alloc;

      return new (object) TYPE();
   }

   //! Deallocate an object
   void free(TYPE* object_)
   {
      object_->~TYPE();

      object_->next = free_list;
      free_list     = object_;

      --num_alloc;
   }

private:
   TYPE   heap[SIZE];
   TYPE*  free_list{};
   size_t num_alloc{SIZE};
};

} // namespace STB
