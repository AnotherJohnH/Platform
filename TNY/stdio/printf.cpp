//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>

#include "UCL/PrintF.h"

namespace {

class Buffer : public PrintF
{
public:
    Buffer() = default;

private:
    void putc(char ch) override
    {
       putchar(ch);
       ++count;
    }
};

} // namespace

int vprintf(const char* format, va_list ap)
{
   Buffer buffer;
   return buffer.vprintf(format, ap);
}

int printf(const char* format, ...)
{
   va_list ap;
   va_start(ap, format);
   int status = vprintf(format, ap);
   va_end(ap);
   return status;
}
