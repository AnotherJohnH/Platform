//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief A list of elements that are in a singleton list

#pragma once

namespace STB {

namespace SingletonList {

//! List element base class
template <typename TYPE, bool CONSTRUCTION_ORDER = false>
class Element
{
public:
   Element()
   {
      // Elements are automatically put into the list on construction
      if (CONSTRUCTION_ORDER)
         push_back();
      else
         push_front();
   }

   //! Access to the front of the list
   static TYPE*& front()
   {
      static TYPE* list = nullptr;
      return list;
   }

   //! Get the next element to this one
   TYPE* next() { return next_ptr; }

   //! Get the next element to this one
   const TYPE* next() const { return next_ptr; }

private:
   //! Push this element on the front of the list O(1)
   void push_front()
   {
      next_ptr = front();
      front() = static_cast<TYPE*>(this);
   }

   //! Push this element on the back of the list O(n)
   void push_back()
   {
      TYPE* element = front();
      if(element == nullptr)
      {
         front() = static_cast<TYPE*>(this);
      }
      else
      {
         while(element->next())
         {
            element = element->next();
         }
         element->next_ptr = static_cast<TYPE*>(this);
      }
   }

   TYPE* next_ptr{nullptr};
};

} // namespace SingletonList

} // namespace STB

