//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef STB_TREE_H
#define STB_TREE_H

namespace STB {

//! Tree data structure
template <class TYPE>
class Tree
{
public:
   //------------------------------------------------------------------
   // Accessors

   //! Returns true if the node has no children
   bool empty() const { return children == nullptr; }

   TYPE* getParent() const { return parent; }

   //------------------------------------------------------------------
   // Modifiers

   void set_parent(TYPE* parent_)
   {
      parent = parent_;
   }

   //! Add a new child element into the given element
   void push_back(TYPE* child)
   {
      for(TYPE** ptr = &children; true; ptr = &((*ptr)->next))
      {
         if (*ptr == nullptr)
         {
            *ptr = child;
            break;
         }
      }
   }

protected:
   TYPE*  parent{nullptr};
   TYPE*  next{nullptr};
   TYPE*  children{nullptr};
};


} // namespace STB

#endif
