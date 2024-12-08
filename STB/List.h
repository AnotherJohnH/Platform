//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief Singly linked ordered list

#pragma once

#include <cassert>

namespace STB {

//! Singly linked list without extra node objects
template <typename TYPE>
class List
{
public:
   //! Base class for list elems
   class Elem
   {
   friend class List;

   public:
      TYPE* nextElem() const { return next; }

   private:
      TYPE* next{nullptr};
   };

   template <typename T>
   class iterator_impl
   {
   public:
      iterator_impl(T* ptr_)
         : ptr(ptr_)
      {}

      iterator_impl& operator++()
      {
         ptr = ptr->next;
         return *this;
      }

      bool operator==(const iterator_impl& other) const { return ptr == other.ptr; }

      bool operator!=(const iterator_impl& other) const { return ptr != other.ptr; }

      T& operator*() { return *ptr; }

      T* ptr;
   };

   using iterator       = iterator_impl<TYPE>;
   using const_iterator = iterator_impl<const TYPE>;

   //! Test for empty list
   bool empty() const  { return first == nullptr; }

   //! Return the number of elems in the list
   size_t size() const { return n; }

   //! Returns pointer to the first item in the list
   TYPE* front() const { return first; }

   //! Returns pointer to the last item in the list
   //  XXX O(n)
   TYPE* back() const
   {
      TYPE* back;

      for(back = first; back->next != nullptr; back = back->next);

      return back;
   }

   //! Return an iterator for the first elem in the list
   iterator begin() { return iterator(first); }

   //! return an iterator for the end of the list
   iterator end() { return iterator(nullptr); }

   //! Return an iterator for the first elem in the list
   const_iterator begin() const { return const_iterator(first); }

   //! return an iterator for the end of the list
   const_iterator end() const { return const_iterator(nullptr); }


   //! Clear list to empty
   void clear()
   {
      first = nullptr;
      n     = 0;
   }

   //! Remove the first elem from the list
   void pop_front()
   {
      assert(not empty());

      first = first->next;
      --n;
   }

   //! Remove the last elem from the list
   void pop_back()
   {
      assert(not empty());

      remove(back());
   }

   //! Remove an elem from the list
   //  XXX O(n)
   void remove(TYPE* elem)
   {
      TYPE** prev = &first;

      for(TYPE* e = first; e; e = e->next)
      {
         if (elem == e)
         {
            *prev = e->next;
            e->next = nullptr;
            --n;
            return;
         }

         prev = &e->next;
      }
   }

   //! Insert a new elem at the front of the list
   void push_front(TYPE* elem)
   {
      assert(elem != first);

      elem->next = first;
      first = elem;

      ++n;
   }

   //! Insert a new elem at the back of the list
   //  XXX O(n)
   void push_back(TYPE* elem)
   {
      elem->next = nullptr;

      if (first == nullptr)
      {
         first = elem;
      }
      else
      {
         back()->next = elem;
      }

      ++n;
   }

private:
   TYPE*  first{nullptr};
   size_t n{0};
};

} // namespace STB

