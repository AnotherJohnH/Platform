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

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <string>
#include <vector>

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

   bool write(FILE* fp) const
   {
      return fwrite(this, sizeof(TYPE), 1, fp) == 1;
   }
};

//! IFF 4-char ident
class Ident : public FileIo<Ident>
{
private:
   char value[4];

   static_assert(sizeof(value) == 4,
                 "This class represents a 4-byte field in a binary file.");

public:
   Ident(const std::string& value_ = "    ")
   {
      strncpy(value, value_.c_str(), sizeof(value));
   }

   void get(std::string& v) const
   {
      v.clear();

      for(unsigned i=0; i<4; i++)
         v.push_back(value[i]);
   }

   bool operator==(const std::string& rhs) const
   {
      return strncmp(value, rhs.c_str(), sizeof(value)) == 0;
   }

   bool operator!=(const std::string& rhs) const
   {
      return !operator==(rhs);
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

   uint32_t operator=(uint32_t value_) { value = value_; return value_; }
};

//! IFF chunk
class Chunk
{
public:
   Chunk(const std::string& type_, size_t reserve_ = 0)
      : type(type_)
   {
      bytes.reserve(reserve_);
   }

   //! Get chunk type
   const Ident& getType() const { return type; }

   //! Get chunk size
   uint32_t getSize() const { return size; }

   //! Get total chunk file size
   size_t getFileSize() const
   {
      size_t file_size = size;
      // Ensure size is an even number
      if (file_size & 1) file_size++;
      return file_size + 8;
   }

   bool empty() const { return bytes.empty(); }

   //! Get chunk data
   const void* data() const { return bytes.data(); }

   //! Allocate chunk data
   void* alloc()
   {
      bytes.resize(size);
      return bytes.data();
   }

   //! Read chunk header and skip to next chunk
   bool read(FILE* fp)
   {
      bool ok = false;

      if (type.read(fp) && size.read(fp))
      {
         ok = fseek(fp, getFileSize() - 8, SEEK_CUR) == 0;
      }

      return ok;
   }

   //! Write chunk
   bool write(FILE* fp) const
   {
      type.write(fp);
      size.write(fp);

      if (!bytes.empty())
      {
         if (fwrite(bytes.data(), bytes.size(), 1, fp) != 1) return false;

         if (bytes.size() & 1)
         {
            const uint8_t zero = 0;
            if (fwrite(&zero, 1, 1, fp) != 1) return false;
         }
      }

      return true;
   }

   //! Add raw data to chunk
   void push(const void* data_ptr, size_t n)
   {
      size_t end = bytes.size();
      bytes.resize(end + n);
      size = bytes.size();
      memcpy(&bytes[end], data_ptr, n);
   }

   //! Add object to chunk
   template <typename TYPE>
   void push(const TYPE& data)
   {
      push(&data, sizeof(TYPE));
   }

   void clear()
   {
      bytes.clear();
      size = 0;
   }

private:
   Ident                 type;
   UInt32                size;
   std::vector<uint8_t>  bytes;
};


//! IFF document
class Document
{
public:
   Document() = default;

   Document(const std::string& document_type_, const std::string& file_type_)
      : document_type(document_type_)
      , file_type(file_type_)
   {
   }

   ~Document()
   {
      if (fp != nullptr) fclose(fp);
   }

   //! Test document type
   bool isDocType(const std::string& type) const { return document_type == type; }

   //! Test file type
   bool isFileType(const std::string& type) const { return file_type == type; }

   //! Add a new chunk
   Chunk* newChunk(const std::string& type, size_t reserve = 0)
   {
      Chunk* chunk = findChunk(type);
      if (chunk != nullptr)
      {
         chunk->clear();
         return chunk;
      }
      chunk_list.emplace_back(type, reserve);
      return &chunk_list.back();
   }

   //! Find a chunk by type
   Chunk* findChunk(const std::string& type)
   {
      for(auto& chunk : chunk_list)
      {
         if (chunk.getType() == type)
         {
            return &chunk;
         }
      }

      return nullptr;
   }

   //! Find a chunk by offset
   Chunk* findChunk(uint32_t offset_)
   {
      size_t offset = 12;

      for(auto& chunk : chunk_list)
      {
         if (offset == offset_)
         {
            return &chunk;
         }

         offset += chunk.getFileSize();
      }

      if (fseek(fp, offset_, SEEK_SET) == 0)
      {
         Chunk* chunk = newChunk("    ");
         if (chunk->read(fp))
         {
            return chunk;
         }
      }

      return nullptr;
   }

   //! Read a document
   bool read(const std::string& filename,
             const std::string& doc_type_,
             const std::string& file_type_)
   {
      clear();

      if (!open(filename, "r")) return false;

      bool ok = false;

      if (document_type.read(fp) &&
          isDocType(doc_type_) &&
          file_size.read(fp) &&
          file_type.read(fp) &&
          isFileType(file_type_))
      {
         ok = true;

         size_t size = 4;

         while(size < file_size)
         {
            Chunk* chunk = newChunk("    ");
            if (!chunk->read(fp))
            {
               ok = false;
               break;
            }

            size += chunk->getFileSize();
         }
      }

      return ok;
   }

   //! Write a document
   bool write(const std::string& filename)
   {
      if (!open(filename, "w")) return false;

      size_t size = 4;
      for(const auto& chunk : chunk_list)
      {
         size += chunk.getFileSize();
      }
      file_size = uint32_t(size);

      bool ok = false;

      if (document_type.write(fp) &&
          file_size.write(fp) &&
          file_type.write(fp))
      {
         ok = true;

         for(const auto& chunk : chunk_list)
         {
            if(!chunk.write(fp))
            {
               ok = false;
               break;
            }
         }
      }

      return ok;
   }

   //! Load the named chunk
   template <typename TYPE>
   const TYPE* load(const std::string& type, uint32_t* size = nullptr)
   {
      size_t offset = 12;

      for(auto& chunk : chunk_list)
      {
         if (chunk.getType() == type)
         {
            if (chunk.empty() && (fp != nullptr))
            {
               // Try and load the chunk from disk
               void* data = chunk.alloc();
               if ((fseek(fp, offset + 8, SEEK_SET) != 0) ||
                   (fread(data, chunk.getSize(), 1, fp) != 1))
               {
                  chunk.clear();
               }
            }

            if (!chunk.empty() && (size != nullptr))
            {
               *size = chunk.getSize();
            }

            return static_cast<const TYPE*>(chunk.data());
         }

         offset += chunk.getFileSize();
      }

      return nullptr;
   }

   void clear()
   {
      chunk_list.clear();
   }

private:
   FILE*              fp{nullptr};
   Ident              document_type{};
   UInt32             file_size{0};
   Ident              file_type{};
   std::vector<Chunk> chunk_list;

   bool open(const std::string& filename, const char* mode)
   {
      if (fp != nullptr) fclose(fp);
      fp = fopen(filename.c_str(), mode);
      return fp != nullptr;
   }
};

} // namespace IFF

} // namespace STB

#endif // STB_IFF_H
