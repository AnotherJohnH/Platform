//------------------------------------------------------------------------------
// Copyright (c) 2012-2019 John D. Haughton
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

#ifndef XML_LIST_H
#define XML_LIST_H

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

#endif
