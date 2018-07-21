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

namespace PLT {

//! A file stream
class File
{
public:
   //! Open a file stream
   //
   // \param filename full filename
   // \param mode fopen() style mode
   File(const char* filename, const char* mode);

   //! Open a file stream
   //
   // \param filename filename
   // \param ext filename extension
   // \param mode fopen() style mode
   File(const char* filename, const char* ext, const char* mode);

   //! Close the file stream
   ~File();

   //! Return the filename
   const char* getFilename() const;

   //! Return true if the file is open
   bool isOpen() const;

   //! Return true if the file is not open or at the end of the file
   bool isEof() const;

   //! Get next character from input stream
   bool getChar(char& ch);

   //! Get next line from input stream
   bool getLine(char* buffer, size_t max);

   //! Seek to the given file offset
   bool seek(size_t offset);

   //! Read raw data
   bool read(void* data, size_t size);

   //! Formated output
   void print(const char* format, ...);

   //! Write raw data
   bool write(const void* data, size_t size);

   //! Flush an output stream
   void flush();

   //! Report an error
   bool error(const char* format, ...);

   //! Create a directory
   static bool createDir(const char* path);

private:
   class Impl;

   Impl* pimpl{nullptr};
};


} // namespace STB 

#endif // PLT_FILE_H
