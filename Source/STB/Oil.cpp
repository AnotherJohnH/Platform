//------------------------------------------------------------------------------
// Copyright (c) 2015-2017 John D. Haughton
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

#include "PLT/File.h"
#include "STB/Lex.h"
#include "STB/Oil.h"


namespace STB {

namespace OIL {

void Member::write(PLT::File& file, void* that) const
{
   file.print("   %s=\"", name);

   if (elements > 1) file.print("[");

   for(size_t i=0; i<elements; ++i)
   {
      if (i > 0) file.print(", ");

      void* data = (uint8_t*)that + offset + size*i;

      switch(type)
      {
      case Type::BOOL:
         file.print("%s", *(bool*)data ? "true" : "false");
         break;

      case Type::SIGNED:
         switch(size)
         {
         case 1: file.print("%d",   *(int8_t*)data); break;
         case 2: file.print("%d",   *(int16_t*)data); break;
         case 4: file.print("%d",   *(int32_t*)data); break;
         case 8: file.print("%lld", *(int64_t*)data); break;
         default: assert(!"unexpected member size"); break;
         }
         break;

      case Type::ENUM:
      case Type::UNSIGNED:
      {
          bool hex = (flags & HEX) != 0;

             switch(size)
             {
             case 1: file.print(hex ? "0x%02x"    : "%u",   *(uint8_t*)data);  break;
             case 2: file.print(hex ? "0x%04x"    : "%u",   *(uint16_t*)data); break;
             case 4: file.print(hex ? "0x%08x"    : "%u",   *(uint32_t*)data); break;
             case 8: file.print(hex ? "0x%016llx" : "%llu", *(uint64_t*)data); break;
             default: assert(!"unexpected member size"); break;
             }
      }
      break;

      case Type::FLOAT:
         switch(size)
         {
         case 4: file.print("%g", *(float*)data); break;
         case 8: file.print("%g", *(double*)data); break;
         default: assert(!"unexpected member size"); break;
         }
         break;
      }
   }

   if (elements > 1) file.print("]");

   file.print("\"\n");
}

void ClassBase::write(PLT::File& file, void* that) const
{
   file.print("<%s\n", name);

   for(const auto& member : member_list)
   {
      member.write(file, that);
   }

   file.print("/>\n");
}

void ClassBase::write(void* that) const
{
   PLT::File file(name, "xml", "w");

   write(file, that);
}


bool Member::read(Lex& lex, void* that) const
{
   if (elements > 1)
   {
      if (!lex.match('[')) return false;
   }

   for(size_t i=0; i<elements; ++i)
   {
      if (i > 0)
      {
         if (!lex.match(',')) return false;
      }

      void* data = (uint8_t*)that + offset + size*i;

      switch(type)
      {
      case Type::BOOL:
         if (lex.isMatch("true"))
         {
            *(bool*)data = true;
         }
         else if (lex.isMatch("false"))
         {
            *(bool*)data = false;
         }
         else
         {
            return lex.error("Expected boolean value");
         }
         break;

      case Type::SIGNED:
      {
         int64_t value = 0;
         if (!lex.matchSigned(value)) return false;
         int64_t unused = value >> (size*8);
         if ((unused != 0) || (unused != -1))
         {
            return lex.error("signed integer value too big");
         }
         switch(size)
         {
         case 1: *(int8_t*)data = int8_t(value); break;
         case 2: *(int16_t*)data = int16_t(value); break;
         case 4: *(int32_t*)data = int32_t(value); break;
         case 8: *(int64_t*)data = value; break;
         }
      }
      break;

      case Type::ENUM:
      case Type::UNSIGNED:
      {
         uint64_t value = 0;
         if (!lex.matchUnsigned(value)) return false;
         uint64_t unused = value >> (size*8);
         if (unused != 0)
         {
            return lex.error("unsigned integer value too big");
         }
         switch(size)
         {
         case 1: *(uint8_t*)data = uint8_t(value); break;
         case 2: *(uint16_t*)data = uint32_t(value); break;
         case 4: *(uint32_t*)data = uint32_t(value); break;
         case 8: *(uint64_t*)data = value; break;
         }
      }
      break;

      case Type::FLOAT:
      {
         double value = 0.0;
         if (!lex.matchFloat(value)) return false;
         switch(size)
         {
         case 4: *(float*)data = float(value); break;
         case 8: *(double*)data = value; break;
         }
      }
      break;
      }
   }

   if (elements > 1)
   {
      if (!lex.match(']')) return false;
   }

   return true;
}

bool ClassBase::read(Lex& lex, void* that) const
{
   if (!lex.match('<')) return false;

   if (!lex.match(name)) return false;

   while(!lex.isMatch("/>"))
   {
      std::string ident;
      if (!lex.matchIdent(ident)) return false;

      const Member* member = findMember(ident.c_str());
      if (member == nullptr) return lex.error("bad member name '%s'", ident.c_str());

      if (!lex.match('=')) return false;

      if (!lex.match('"')) return false;

      if (!member->read(lex, that)) return false;

      if (!lex.match('"')) return false;
   }

   return true;
}

bool ClassBase::read(void* that) const
{
   LEX::File lex(name, "xml");
   if (lex.isEof()) return false;

   return read(lex, that);
}

} // namespace OIL

} // namespace STB
