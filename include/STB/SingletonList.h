//------------------------------------------------------------------------------
// Copyright (c) 2015-2017 John D. Haughton
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

// \brief A list of elements that are in a singleton list

#ifndef STB_SINGLETON_LIST_H
#define STB_SINGLETON_LIST_H

namespace STB {

namespace SingletonList {

//! List element base class
template <typename TYPE>
class Element
{
public:
   Element()
   {
      push_back();
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

#endif
