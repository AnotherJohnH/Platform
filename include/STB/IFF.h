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

#ifndef STB_IFF_H
#define STB_IFF_H

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "STB/Endian.h"

namespace STB {

//! Support for Interchange File Format
namespace IFF {

//! File I/O base class
template <typename TYPE>
class FileIo
{
public:
   bool read(FILE* fp)
   {
      return fread(this, sizeof(TYPE), 1, fp) == 1;
   }

   bool write(FILE* fp)
   {
      return fwrite(this, sizeof(TYPE), 1, fp) == 1;
   }
};

//! IFF 4-char ident
class Ident : public FileIo<Ident>
{
private:
   char value[4];

public:
   Ident(const char* value_ = "    ")
   {
      strncpy(value, value_, sizeof(value));
   }

   bool operator==(const char* rhs) const
   {
      return strncmp(value, rhs, sizeof(value)) == 0;
   }

   bool operator!=(const char* rhs) const
   {
      return !operator==(rhs);
   }

   bool match(FILE* fp, const char* expected)
   {
      return read(fp) && operator==(expected);
   }
};

//! IFF 32-bit unsigned integer
class UInt32 : public FileIo<UInt32>
{
private:
   STB::Big32 value;

public:
   UInt32(uint32_t value_ = 0)
      : value(value_)
   {
   }

   operator uint32_t() const { return value; }
};

//! IFF chunk header
struct ChunkHeader : public FileIo<ChunkHeader>
{
   Ident  ident;
   UInt32 size;

   ChunkHeader(const char* ident_ = "    ", uint32_t size_ = 0)
      : ident(ident_)
      , size(size_)
   {
   }

   bool match(FILE* fp, const char* expected)
   {
      return read(fp) && (ident == expected);
   }
};

//! IFF document
class Form
{
public:
   Form(const char* type_ = "    ")
      : file_type(type_)
   {
   }

   bool read(FILE* fp)
   {
      return file_header.match(fp, "FORM") && file_type.read(fp);
   }

   bool match(FILE* fp, const char* expected)
   {
      return read(fp) && (file_type == expected);
   }

private:
   ChunkHeader file_header{"FORM"};
   Ident       file_type;
};

} // namespace IFF

} // namespace STB

#endif // STB_IFF_H
