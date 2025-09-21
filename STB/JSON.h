//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// XXX Not quite JSON no numbers or bools

#pragma once

#include "STB/Lex.h"

#include <string>
#include <vector>

namespace JSON {

//! A JSON element
class Element
{
public:
   enum Type { EMPTY, STRING, ARRAY, OBJECT, KEY_VALUE };

   Element() = default;

   //! Return element type
   Type getType() const { return type; }

   //! Set element as empty
   Element& setEmpty()
   {
      switch(type)
      {
      case STRING: string = "";  break;
      case ARRAY:  list.clear(); break;
      case OBJECT: list.clear(); key_list.clear(); break;

      default: break;
      }

      type = EMPTY;
      return *this;
   }

   //! Set element as a string
   Element& setString(const std::string& string_)
   {
      type   = STRING;
      string = string_;
      return *this;
   }

   //! Set element as an array
   Element& setArray()
   {
      setEmpty();
      type = ARRAY;
      return *this;
   }

   //! Add an element to an object
   Element& setObject()
   {
      setEmpty();
      type = OBJECT;
      return *this;
   }

   //! Add an element to an array
   Element& addElement()
   {
      assert(getType() == ARRAY);

      list.emplace_back();
      return list.back();
   }

   //! Add a field to an object
   Element& addField(const std::string& key_)
   {
      assert(getType() == OBJECT);

      key_list.emplace_back(key_);
      list.emplace_back();
      return list.back();
   }

   const std::string& getString() const
   {
      assert(getType() == STRING);
      return string;
   }

   const std::vector<Element>& getArray() const
   {
      assert(getType() == ARRAY);
      return list;
   }

   std::vector<Element>& getArray()
   {
      assert(getType() == ARRAY);
      return list;
   }

   Element* operator[](const std::string& key_)
   {
      size_t index = 0;

      for(const auto& key : key_list)
      {
         if (key == key_)
         {
            return &list[index];
         }
      }

      return nullptr;
   }

   const Element* operator[](const std::string& key_) const
   {
      size_t index = 0;

      for(const auto& key : key_list)
      {
         if (key == key_)
         {
            return &list[index];
         }
         ++index;
      }

      return nullptr;
   }

protected:
   bool read(STB::Lex& lex)
   {
      std::string value;

      if (lex.isMatch('['))
      {
         setArray();

         while(true)
         {
            Element& element = addElement();

            if (not element.read(lex)) return false;

            if (not lex.isMatch(',')) break;
         }

         if (not lex.match(']')) return false;
      }
      else if (lex.isMatch('{'))
      {
         setObject();

         std::string key;

         while(lex.isMatchString(key))
         {
            Element& element = addField(key);

            if (not lex.match(':')) return false;
            
            if (not element.read(lex)) return false;

            if (not lex.isMatch(',')) break;
         }

         if (not lex.match('}')) return false;
      }
      else if (lex.isMatch("null"))
      {
         setEmpty();
      }
      else if (lex.isMatchString(value))
      {
         setString(value);
      }
      else
      {
         return false;
      }

      return true;
   }

   static void indent(FILE* fp, unsigned indent_)
   {
      for(unsigned i = 0; i < indent_; ++i)
      {
         fprintf(fp, "  ");
      }
   }

   void write(FILE* fp, unsigned indent_ = 0) const
   {
      bool   comma{false};
      size_t index = 0;

      indent(fp, indent_);

      switch(type)
      {
      case EMPTY:
         fprintf(fp, "null");
         break;

      case STRING:
         fprintf(fp, "\"%s\"", string.c_str());
         break;

      case ARRAY:
         comma = false;
         fprintf(fp, "[\n");
         for(const auto& element : list)
         {
            if (comma)
               fprintf(fp, ",\n");
            else
               comma = true;
            element.write(fp, indent_ + 1);
         }
         fprintf(fp, "\n");
         indent(fp, indent_);
         fprintf(fp, "]");
         break;

      case OBJECT:
         comma = false;
         fprintf(fp, "{\n");
         for(const auto& element : list)
         {
            if (comma)
               fprintf(fp, ",\n");
            else
               comma = true;
            indent(fp, indent_ + 1);
            fprintf(fp, "\"%s\" : ", key_list[index].c_str());
            element.write(fp, indent_ + 1);
            ++index;
         }
         fprintf(fp, "\n");
         indent(fp, indent_);
         fprintf(fp, "}");
         break;

      default:
         break;
      }
   }

   Type                     type;
   std::string              string{};
   std::vector<Element>     list{};
   std::vector<std::string> key_list{};
};


//! A JSON document
class Document : public Element
{
public:
   //! Construct a JSON document from a file
   Document(const std::string& filename)
   {
      STB::LEX::File lex(filename.c_str());

      if (lex.isOpen())
      {
         ok = read(lex);
      }
   }

   //! Construct an empty XML document
   Document()
      : ok{true}
   {}

   bool isOk() const { return ok; }

   void write(FILE* fp) const
   {
      Element::write(fp, 0);
   }

private:
   bool ok{false};
};

} // namespace JSON

