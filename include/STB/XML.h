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

#ifndef STB_XML_H
#define STB_XML_H

#include "STB/Lex.h"

#include <string>
#include <vector>


namespace STB {

namespace XML {

//! An XML element attribute
class Attr
{
public:
   Attr(const std::string& name_)
      : name(name_)
   {}

   //! Get name of attribute
   std::string getName() const { return name; }

   //! Get value of attribute
   std::string getValue() const { return value; }

   //! Get reference to the value of the attribute
   void set(const std::string& value_) { value = value_; }

   //! Read an attribute value from a Lex stream
   void parse(Lex& lex)
   {
      lex.match('=');
      lex.matchString(value);
   }

   void write(FILE* fp) const
   {
      fprintf(fp, " %s=\"%s\"", name.c_str(), value.c_str());
   }

private:
   std::string name{};
   std::string value{};
};


//! An XML element
class Element : public std::vector<Element>
{
public:
   Element() = default;

   Element(const std::string& type_)
      : type(type_)
   {}

   //! Get type of element
   std::string getName() const { return type; }

   //! Get value of element
   std::string getValue() const { return value; }

   //! Get list of attributes for this element
   const std::vector<Attr>& getAttrList() const { return attr_list; }

   //! Return value of a named attribute or empty string
   std::string operator[](const std::string& attr_name) const
   {
      const Attr* attr = findAttr(attr_name);
      return attr != nullptr ? attr->getValue() : "";
   }

   //! Check if element has the named attribute
   bool hasAttr(const std::string& attr_name) const
   {
      return findAttr(attr_name) != nullptr;
   }

   //! Extract a single bool value from the named attribute
   void match(const std::string& attr_name, bool& value) const
   {
       LEX::String(operator[](attr_name)).match(value);
   }

   //! Extract a single char value from the named attribute
   void match(const std::string& attr_name, char& value) const
   {
       value = operator[](attr_name)[0];
   }

   //! Extract a single unsigned integer value from the named attribute
   void match(const std::string& attr_name, unsigned& value) const
   {
       LEX::String(operator[](attr_name)).matchUnsigned(value);
   }

   //! Extract a pair of unsigned integer values from the named attribute
   void match(const std::string& attr_name,
              unsigned&          value1,
              unsigned&          value2) const
   {
       LEX::String lex(operator[](attr_name));
       lex.matchUnsigned(value1);
       lex.matchUnsigned(value2);
   }

   //! Extract a single signed integer value from the named attribute
   void match(const std::string& attr_name, signed& value) const
   {
       LEX::String(operator[](attr_name)).matchSigned(value);
   }

   //! Extract a pair of signed integer values from the named attribute
   void match(const std::string& attr_name,
              signed&            value1,
              signed&            value2) const
   {
       LEX::String lex(operator[](attr_name));
       lex.matchSigned(value1);
       lex.matchSigned(value2);
   }

   //! Extract a single floating-point value from the named attribute
   void match(const std::string& attr_name, double& value) const
   {
       LEX::String(operator[](attr_name)).matchFloat(value);
   }

   //! Extract a pair of floating-point values from the named attribute
   void match(const std::string& attr_name,
              double&            value1,
              double&            value2) const
   {
       LEX::String lex(operator[](attr_name));
       lex.matchFloat(value1);
       lex.matchFloat(value2);
   }

   //! Extract a single string value from the named attribute
   void match(const std::string& attr_name, std::string& value) const
   {
       value = operator[](attr_name);
   }

   void setType(const std::string& type_)
   {
      type = type_;
   }

   void set(const std::string& attr_name, const std::string& value)
   {
       Attr* attr = findAttr(attr_name);
       if (attr == nullptr)
       {
          attr_list.emplace_back(attr_name);
          attr = &attr_list.back();
       }
       attr->set(value);
   }

   void setBool(const std::string& attr_name, bool value_)
   {
       set(attr_name, value_ ? "true" : "false");
   }

   void setChar(const std::string& attr_name, char value_)
   {
       std::string value{value_};
       set(attr_name, value);
   }

   void setUnsigned(const std::string& attr_name, unsigned value)
   {
       set(attr_name, std::to_string(value));
   }

   void setUnsigned(const std::string& attr_name, unsigned value1, unsigned value2)
   {
       std::string value = std::to_string(value1);
       value += ' ';
       value += std::to_string(value2);
       set(attr_name, value);
   }

   void setSigned(const std::string& attr_name, signed value)
   {
       set(attr_name, std::to_string(value));
   }

   void setDouble(const std::string& attr_name, double value)
   {
       set(attr_name, std::to_string(value));
   }

   void setDouble(const std::string& attr_name, double value1, double value2)
   {
       std::string value = std::to_string(value1);
       value += ' ';
       value += std::to_string(value2);
       set(attr_name, value);
   }

   STB::XML::Element* add(const std::string& name)
   {
      emplace_back(name);
      return &back();
   }

   void write(FILE* fp, unsigned indent=0) const
   {
      for(unsigned i=0; i<indent; i++)
      {
         fprintf(fp, "   ");
      }

      fprintf(fp, "<%s", type.c_str());

      for(const auto& attr : attr_list)
      {
         attr.write(fp);
      }

      if (empty())
      {
         fprintf(fp, "/");
      }
      else
      {
         fprintf(fp, ">\n");

         for(const auto& child : *this)
         {
            child.write(fp, indent + 1);
         }

         for(unsigned i=0; i<indent; i++)
         {
            fprintf(fp, "   ");
         }

         fprintf(fp, "</%s", type.c_str());
      }
      fprintf(fp, ">\n");
   }

protected:
   bool parse(Lex& lex)
   {
      lex.matchIdent(type);

      std::string attr_name;
      while(lex.isMatchIdent(attr_name))
      {
         attr_list.emplace_back(attr_name);
         attr_list.back().parse(lex);
      }

      if (lex.isMatch('/'))
      {
         lex.match('>');
         return true;
      }

      lex.match('>');

      while(true)
      {
         if (lex.isMatch('<'))
         {
            if (lex.isMatch('/')) break;

            emplace_back();
            if (!back().parse(lex)) return true;
         }
         else
         {
            value += lex.next();
            lex.sink();
         }
      }

      if (!lex.isMatch(type.c_str()))
      {
         lex.error("</%s> expected", type.c_str());
         std::string term;
         lex.matchIdent(term);
         lex.match('>');
         return false;
      }

      lex.match('>');
      return true;
   }

private:
   Attr* findAttr(const std::string& attr_name)
   {
      for(auto& attr : attr_list)
      {
         if (attr.getName() == attr_name)
         {
            return &attr;
         }
      }

      return nullptr;
   }

   const Attr* findAttr(const std::string& attr_name) const
   {
      for(const auto& attr : attr_list)
      {
         if (attr.getName() == attr_name)
         {
            return &attr;
         }
      }

      return nullptr;
   }

   std::string       type{};
   std::string       value{};
   std::vector<Attr> attr_list{};
};


//! The root element in an XML document
class Document : public Element
{
public:
   //! Construct an XML document from a file
   Document(const std::string& filename, bool require_prologue = false)
   {
      LEX::File lex(filename.c_str());

      if (lex.isOpen())
      {
         lex.setSpecialIdentChar("_-:");
         lex.setComment("<!--", "-->");

         ok = parseDocument(lex, require_prologue);
      }
   }

   //! Construct an empty XML document
   Document()
      : ok{true}
      , version("1.1")
      , encoding("UTF-8")
      , standalone("no")
   {}

   bool isOk() const { return ok; }

   //! Write document to an open filestream
   void write(FILE* fp) const
   {
      fprintf(fp, "<?xml version=\"%s\"", version.c_str());
      if (!encoding.empty())   fprintf(fp, " encoding=\"%s\"",   encoding.c_str());
      if (!standalone.empty()) fprintf(fp, " standalone=\"%s\"", standalone.c_str());
      fprintf(fp, "?>\n");

      if (!doctype.empty())
      {
         fprintf(fp, "<!DOCTYPE %s", doctype.c_str());

         if (!system_literal.empty())
         {
            if (public_literal.empty())
            {
               fprintf(fp, " SYSTEM \"%s\"", system_literal.c_str());
            }
            else
            {
               fprintf(fp, " PUBLIC \"%s\" \"%s\"",
                       public_literal.c_str(),
                       system_literal.c_str());
            }
         }

         fprintf(fp, ">\n");
      }

      Element::write(fp);
   }

private:
   bool parseXMLDecl(Lex& lex, bool require_prolog)
   {
      if (require_prolog)
      {
         lex.match("<?xml");
      }
      else if (!lex.isMatch("<?xml"))
      {
         return true;
      }

      lex.match("version");
      lex.match('=');
      lex.matchString(version);

      if (lex.isMatch("encoding"))
      {
         lex.match('=');
         lex.matchString(encoding);
      }

      if (lex.isMatch("standalone"))
      {
         lex.match('=');
         lex.matchString(standalone);
      }

      return lex.match("?>");
   }

   bool parseProlog(Lex& lex, bool require_prolog)
   {
      parseXMLDecl(lex, require_prolog);

      if (lex.isMatch("<!DOCTYPE"))
      {
         lex.matchIdent(doctype);

         if (lex.isMatch("SYSTEM"))
         {
            lex.matchString(system_literal);
         }
         else if (lex.isMatch("PUBLIC"))
         {
            lex.matchString(public_literal);
            (void) lex.isMatchString(system_literal);
         }

         lex.match('>');
      }

      return true;
   }

   bool parseDocument(Lex& lex, bool require_prolog)
   {
      if (!parseProlog(lex, require_prolog))
      {
         return false;
      }

      if (!lex.match('<'))
      {
         return false;
      }

      return parse(lex);
   }

   bool         ok{false};
   std::string  version;
   std::string  encoding;
   std::string  standalone;
   std::string  doctype;
   std::string  system_literal;
   std::string  public_literal;
   Element      root;
};

} // namespace XML

} // namespace STB

#endif
