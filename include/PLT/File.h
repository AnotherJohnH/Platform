//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

//! \file File.h
//! \brief Platform abstraction interface for file I/O

#ifndef PLT_FILE_H
#define PLT_FILE_H

#include <cstddef>
#include <cstdarg>

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

   //! Create a directory
   static bool exists(const char* path);

   //! Create a directory
   static bool createDir(const char* path);

private:
   class Impl;

   Impl* pimpl{nullptr};
};


} // namespace STB 

#endif // PLT_FILE_H
