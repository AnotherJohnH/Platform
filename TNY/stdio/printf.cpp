//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>

#include "UCL/PrintF.h"

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

int vprintf(const char* format, va_list ap)
{
   Buffer buffer;

   buffer.vprintf(format, ap);

   return buffer.size();
}

int printf(const char* format, ...)
{
   va_list ap;

   va_start(ap, format);
   int status = vprintf(format, ap);
   va_end(ap);

   return status;
}
