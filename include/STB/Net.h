//------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
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

#ifndef STB_NET_H
#define STB_NET_H

namespace STB {


//! A base class for objects that are connected together in a net
template <typename TYPE>
class Net
{
public:
   //! An iterator for net elements
   class iterator
   {
   public:
      iterator(TYPE* ptr_)
         : ptr(ptr_)
      {}

      iterator& operator++()
      {
         ptr = ptr->Net::next;
         return *this;
      }

      bool operator==(const iterator& other) const { return ptr == other.ptr; }

      bool operator!=(const iterator& other) const { return ptr != other.ptr; }

      TYPE& operator*() { return *ptr; }
     
   private:
      TYPE* ptr;
   };

   //! A container of all the other elements in the net
   TYPE& net() { return *(TYPE*)this; }

   //! Return an iterator for the next element in the net
   iterator begin()
   {
      return iterator(next);
   }

   //! return an iterator for the last (actually this) element in the net
   iterator end()
   {
      return iterator((TYPE*)this);
   }

   //! Add an element into the net
   void operator>>(TYPE& insert)
   {
      TYPE* temp = next;
      next = insert.Net::next;
      insert.Net::next = temp;
   }

protected:
   Net() {}

private:
   TYPE*  next{(TYPE*)this};
};


} // namespace STB

#endif
