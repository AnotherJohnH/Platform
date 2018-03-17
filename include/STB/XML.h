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

   const std::string& getName() const { return name; }

   const std::string& getValue() const { return value; }

   void parse(STB::Lex& lex)
   {
      lex.match('=');
      lex.match(value);
   }

private:
   std::string  name;
   std::string  value{};
};


//! An XML element
class Element : public std::vector<Element>
{
public:
   Element(const std::string& = "")
   {}

   const std::string& getName() const { return name; }

   std::string operator[](const std::string& attr_name) const
   {
      for(const auto& attr : attr_list)
      {
         if (attr.getName() == attr_name)
         {
            return attr.getValue();
         }
      }

      return "";
   }

   template <typename TYPE>
   void match(const std::string& attr_name, TYPE& value) const
   {
       STB::String(operator[](attr_name)).match(value);
   }

   void write(FILE* fp, unsigned indent=0) const
   {
      for(unsigned i=0; i<indent; i++)
      {
         fprintf(fp, "   ");
      }

      fprintf(fp, "<%s", name.c_str());

      for(const auto& attr : attr_list)
      {
         fprintf(fp, " %s=\"%s\"", 
                 attr.getName().c_str(),
                 attr.getValue().c_str());
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

         fprintf(fp, "</%s", name.c_str());
      }
      fprintf(fp, ">\n");
   }

protected:
   void parse(STB::Lex& lex)
   {
      lex.matchIdent(name);

      std::string attr_name;
      while(lex.isMatchIdent(attr_name))
      {
         attr_list.emplace_back(attr_name);
         attr_list.back().parse(lex);
      }

      if (lex.isMatch('/'))
      {
         lex.match('>');
         return;
      }

      lex.match('>');

      while(lex.isMatch('<'))
      {
         if (lex.isMatch('/')) break;

         emplace_back();
         back().parse(lex);

      }

      lex.match(name.c_str());
      lex.match('>');
   }

private:
   std::string         name;
   std::vector<Attr>   attr_list;
};


//! The root element in an XML document
class Document : public Element
{
public:
   Document(const std::string& filename, bool require_prologue = false)
   {
      STB::File lex(filename);
      parseDocument(lex, require_prologue);
   }

   Document()
      : version("1.1")
      , encoding("UTF-8")
      , standalone("no")
   {}

   //! Write document to an open filestream
   void write(FILE* fp) const
   {
      fprintf(fp, "<?xml version=\"%s\"", version.c_str());
      if (!encoding.empty())   fprintf(fp, " encoding=\"%s\"",   encoding.c_str());
      if (!standalone.empty()) fprintf(fp, " standalone=\"%s\"", standalone.c_str());
      fprintf(fp, "?>\n");

      if (!type.empty())
      {
         fprintf(fp, "<!DOCTYPE %s", type.c_str());

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
   void parseXMLDecl(STB::Lex& lex, bool require_prolog)
   {
      if (require_prolog)
      {
         lex.match("<?xml");
      }
      else if (!lex.isMatch("<?xml"))
      {
         return;
      }

      lex.match("version");
      lex.match('=');
      lex.match(version);

      if (lex.isMatch("encoding"))
      {
         lex.match('=');
         lex.match(encoding);
      }

      if (lex.isMatch("standalone"))
      {
         lex.match('=');
         lex.match(standalone);
      }

      lex.match("?>");
   }

   void parseProlog(STB::Lex& lex, bool require_prolog)
   {
      parseXMLDecl(lex, require_prolog);

      if (lex.isMatch("<!DOCTYPE"))
      {
         lex.matchIdent(type);

         if (lex.isMatch("SYSTEM"))
         {
            lex.match(system_literal);
         }
         else if (lex.isMatch("PUBLIC"))
         {
            lex.match(public_literal);
            lex.match(system_literal);
         }

         lex.match('>');
      }
   }

   void parseDocument(STB::Lex& lex, bool require_prolog)
   {
      parseProlog(lex, require_prolog);

      lex.match('<');

      parse(lex);
   }

   std::string  version;
   std::string  encoding;
   std::string  standalone;
   std::string  type;
   std::string  system_literal;
   std::string  public_literal;
   Element      root;
};

} // namespace XML

} // namespace STB

#endif
