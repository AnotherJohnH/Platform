//-------------------------------------------------------------------------------
// Copyright (c) 2012 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>

#include "STB/XML.h"

namespace XML {

//! A container class with XML serialisation
template <typename TYPE>
class List
{
public:
   using Container      = std::vector<TYPE*>;
   using iterator       = typename Container::iterator;
   using const_iterator = typename Container::const_iterator;

   List() = default;

   bool fromXML(const Element* xml_list, const std::string& name)
   {
      if (xml_list->getName() != name) return false;

      for(const auto& xml : *xml_list)
      {
         list.push_back(new TYPE(&xml));
      }

      return true;
   }

   void toXML(Element* xml, const std::string& name) const
   {
      if (!list.empty())
      {
         Element* xml_list = xml->add(name);

         for(auto& element : list)
         {
            element->toXML(xml_list);
         }
      }
   }

   ~List()
   {
      for(auto& element : list)
      {
         delete element;
      }
   }

   bool empty() const { return list.empty(); }

   size_t size() const { return list.size(); }

   TYPE* operator[](size_t index) { return list[index]; }

   const TYPE* operator[](size_t index) const { return list[index]; }

   iterator begin() { return list.begin(); }

   iterator end() { return list.end(); }

   const_iterator begin() const { return list.begin(); }

   const_iterator end() const { return list.end(); }

   void push_back(TYPE* element) { list.push_back(element); }

   template<typename... Args>
   void emplace_back(Args&&... args)
   {
      list.push_back(new TYPE(std::forward<Args>(args)...));
   }

   void erase(TYPE* element)
   {
      for(auto it = list.begin(); it != list.end(); it++)
      {
         if (*it == element)
         {
            list.erase(it);
            break;
         }
      }
   }

private:
   Container list;
};

} // namespace XML

