//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file File.h
//! \brief Platform abstraction interface for file I/O

#pragma once

#include <cstddef>
#include <stdarg.h>

namespace PLT {

//! A file stream
class File
{
public:
   //! A file stream
   //
   // \param path optional path or null
   // \param filename filename
   // \param ext optional filename extension or null
   File(const char* path,
        const char* filename,
        const char* ext = nullptr);

   //! Close the file stream
   ~File();

   //! Return the filename
   const char* getFilename() const;

   //! Return true if the file is open
   bool isOpen() const;

   //! Return true if the file is not open or at the end of the file
   bool isEof() const;

   //! Open file
   bool openForRead();

   //! Open file
   bool openForWrite();

   //! Get next character from input stream
   bool getChar(char& ch);

   //! Get next line from input stream
   bool getLine(char* buffer, size_t max);

   //! Seek to the given file offset
   bool seek(size_t offset);

   //! Read raw data
   bool read(void* data, size_t size);

   //! Formated output
   void printf(const char* format, ...)
   {
      va_list ap;
      va_start(ap, format);
      vprintf(format, ap);
      va_end(ap);
   }

   //! Formated output
   void vprintf(const char* format, va_list ap);

   //! Write raw data
   bool write(const void* data, size_t size);

   //! Flush an output stream
   void flush();

   //! Report an error
   bool error(const char* format, ...);

   //! Check for file existance
   static bool exists(const char* path);

   //! Get size of a file
   static size_t size(const char* path);

   //! Create a directory
   static bool createDir(const char* path);

private:
   class Impl;

   Impl* pimpl{nullptr};
};


} // namespace STB 

