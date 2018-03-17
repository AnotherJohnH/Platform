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

#include <cstdio>
#include <cstdarg>
#include <string>

#include "PLT/File.h"

namespace PLT {

//! POSIX implementation of file access
class File::Impl
{
public:
   Impl(const char* filename_, const char* mode_)
      : filename(filename_)
   {
      fp = fopen(getFilename(), mode_);
   }
   
   Impl(const char* filename_, const char* ext_, const char* mode_)
   {
      filename = filename_;
      filename += '.';
      filename += ext_;

      fp = fopen(getFilename(), mode_);
   }
   
   ~Impl()
   {
      if (isOpen()) fclose(fp);
   }
   
   //! Return the filename
   const char* getFilename() const { return filename.c_str(); }
   
   //! Return true if the file is open
   bool isOpen() const { return fp != nullptr; }

   //! Return true if the file is not open or at the end of file
   bool isEof() const { return !isOpen() || feof(fp); }
   
   //! Get next character from input stream
   bool getChar(char& ch)
   {
      if (!isOpen()) return false;

      int c = fgetc(fp);
      if (c == EOF)
      {
         fclose(fp);
         fp = nullptr;
         return false;
      }

      ch = c;
      if (ch == '\n') ++line_no;
      return true;
   }
   
   //! Get next line from input stream
   bool getLine(char* buffer, size_t size)
   {
      return fgets(buffer, size, fp) != nullptr;
   }

   //! Seek to the given file offser
   bool seek(size_t offset)
   {
      return fseek(fp, offset, SEEK_SET) == 0;
   }

   //! Read raw data
   bool read(void* data, size_t bytes)
   {
      return fread(data, bytes, 1, fp) == 1;
   }

   //! Formated output
   void vprint(const char* format, va_list ap)
   {
      vfprintf(fp, format, ap);
   }
   
   //! Write raw data
   bool write(const void* data, size_t bytes)
   {
      return fwrite(data, bytes, 1, fp) == 1;
   }

   //! Flush output stream
   void flush() { fflush(fp); }
   
   //! Report an error
   void verror(const char* format, va_list ap)
   {
      fprintf(stderr, "ERROR %s:%u - ", getFilename(), line_no);
      vfprintf(stderr, format, ap);
      fprintf(stderr, "\n");
   }

private: 
   std::string  filename;
   FILE*        fp{nullptr};
   unsigned     line_no{1};
};


File::File(const char* filename, const char* mode)
   : pimpl(new Impl(filename, mode))
{}

File::File(const char* filename, const char* ext, const char* mode)
   : pimpl(new Impl(filename, ext, mode))
{}

File::~File()
{
   delete pimpl;
}

bool File::isOpen() const { return pimpl->isOpen(); }

bool File::isEof() const { return pimpl->isEof(); }

const char* File::getFilename() const { return pimpl->getFilename(); }

bool File::getChar(char& ch) { return pimpl->getChar(ch); }

bool File::seek(size_t offset) { return pimpl->seek(offset); }

bool File::read(void* data, size_t bytes) { return pimpl->read(data, bytes); }

bool File::getLine(char* buffer, size_t size) { return pimpl->getLine(buffer, size); }

void File::print(const char* format, ...)
{
   va_list ap;
   va_start(ap, format);
   pimpl->vprint(format, ap);
   va_end(ap);
}

bool File::write(const void* data, size_t bytes) { return pimpl->write(data, bytes); }

void File::flush()
{  
   pimpl->flush();
}
   
bool File::error(const char* format, ...)
{
   va_list ap;
   va_start(ap, format);
   pimpl->verror(format, ap);
   va_end(ap);
   return false;
}

} // namespace PLT
