//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <stdio.h>

#include "UCL/PrintF.h"

namespace {

class Buffer : public PrintF
{
public:
    Buffer(FILE* stream_)
       : stream(stream_)
    {
    }

private:
    void putc(char ch) override
    {
       fputc(ch, stream);
       ++count;
    }

    FILE* stream;
};

} // namespace

int vfprintf(FILE* stream, const char* format, va_list ap)
{
   Buffer buffer{stream};
   buffer.vprintf(format, ap);
   return buffer.size();
}

int fprintf(FILE* stream, const char* format, ...)
{
   va_list  ap;

   va_start(ap, format);
   int status = vfprintf(stream, format, ap);
   va_end(ap);

   return status;
}
