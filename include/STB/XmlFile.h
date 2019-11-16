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

#ifndef XML_FILE_H
#define XML_FILE_H

#include <cstdio>

#include "STB/XML.h"

namespace XML {

//! Base class for XML serialisation
template <typename TYPE>
class File 
{
public:
   File() = default;

   virtual ~File() = default;

   virtual void toXML(XML::Element* xml) const = 0;

   virtual std::string getName() const = 0;

   //! Construct an object from an XML file
   static TYPE* read(const std::string& name)
   {
      std::string filename = name;
      filename += ".xml";

      XML::Document xml(filename, /* require_prolog */ false);

      return xml.isOk() ? new TYPE(&xml) : nullptr;
   }

   //! Write object to an XML file
   void write() const
   {
      std::string filename = getName();
      filename += ".xml";

      FILE* fp = fopen(filename.c_str(), "w");
      if (fp != nullptr)
      {
         XML::Document xml;
         toXML(&xml);
         xml.write(fp);

         fclose(fp);
      }
   }
};

} // namespace XML

#endif
