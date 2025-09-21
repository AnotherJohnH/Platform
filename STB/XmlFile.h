//-------------------------------------------------------------------------------
// Copyright (c) 2012 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

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
   static TYPE* read(const std::string& name_)
   {
      std::string filename = name_;

      ensureSuffix(filename);

      XML::Document xml(filename, /* require_prolog */ false);

      return xml.isOk() ? new TYPE(&xml) : nullptr;
   }

   //! Write object to an XML file
   void write() const
   {
      std::string filename = getName();

      ensureSuffix(filename);

      FILE* fp = fopen(filename.c_str(), "w");
      if (fp != nullptr)
      {
         XML::Document xml;
         toXML(&xml);
         xml.write(fp);

         fclose(fp);
      }
   }

   //! Add .xml to filename if missing
   static void ensureSuffix(std::string& filename_)
   {
      if ((filename_.size() < 4) ||
          (filename_.compare(filename_.size() - 4, 4, ".xml") != 0))
      {
         filename_ += ".xml";
      }
   }
};

} // namespace XML
