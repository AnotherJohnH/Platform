//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
