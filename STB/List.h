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
   //! Base class for list elements
   class Element
   {
   friend class List;

   private:
      TYPE* next{nullptr};
   };

   //! Iterator for list elements
   class iterator
   {
   public:
      iterator(TYPE* ptr_)
         : ptr(ptr_)
      {}

      iterator& operator++()
      {
         ptr = ptr->next;
         return *this;
      }

      bool operator==(const iterator& other) const { return ptr == other.ptr; }

      bool operator!=(const iterator& other) const { return ptr != other.ptr; }

      TYPE& operator*() { return *ptr; }

   private:
      TYPE* ptr;
   };

   //! Test for empty list
   bool empty() const  { return first == nullptr; }

   //! Returns pointer to the first item in the list
   TYPE* front() const { return first; }


   //! Return an iterator for the first element in the list
   iterator begin() {  return iterator(first); }

   //! return an iterator for the end of the list
   iterator end() { return iterator(nullptr); }


   //! Remove the first element from the list
   void pop_front()
   {
      assert(!empty());

      first = first->next;
   }

   //! Remove an element from the list
   //  XXX O(n)
   void remove(TYPE* element)
   {
      TYPE** prev = &first;

      for(TYPE* e = first; e; e = e->next)
      {
         if (element == e)
         {
            *prev = e->next;
            e->next = nullptr;
            return;
         }

         prev = &e->next;
      }
   }

   //! Insert a new element at the front of the list
   void push_front(TYPE* element)
   {
      assert(element != first);

      element->next = first;
      first = element;
   }

   //! Insert a new element at the back of the list
   //  XXX O(n)
   void push_back(TYPE* element)
   {
      if (first == nullptr)
      {
         first = element;
      }
      else
      {
         TYPE* prev;

         for(prev = first; prev->next != nullptr; prev = prev->next);

         prev->next = element;
      }
   }

   //! Insert a new element into the list
   //  item must not already be in the list
   void insert(TYPE* element)
   {
      TYPE*  prev = nullptr;
      TYPE*  e;

      for(e = first; e; e = e->next)
      {
         if (*element <= *e) break;

         prev = e;
      }

      assert(element != e);
      element->next = e;

      if (prev)
      {
         assert(prev != element);
         prev->next = element;
      }
      else
      {
         first = element;
      }
   }

private:
   TYPE* first{nullptr};
};

} // namespace STB

