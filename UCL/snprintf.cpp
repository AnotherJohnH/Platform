//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdio.h"
#include "UCL/PrintF.h"

class Buffer : public PrintF
{
public:
    Buffer(char* buffer_, size_t n_)
       : buffer(buffer_)
       , n(n_)
    {
    }

    ~Buffer()
    {
       buffer[i] = '\0';
    }

private:
    void putc(char ch) override
    {
       if (i < (n - 1))
          buffer[i++] = ch;
       ++count;
    }

    char*    buffer;
    unsigned n;
    unsigned i{0};
};

int vsnprintf(char* buffer_, size_t n_, const char* format_, va_list ap_)
{
   Buffer buffer(buffer_, n_);

   buffer.vprintf(format_, ap_);

   return buffer.size();
}

int snprintf(char* buffer, size_t n, const char* format, ...)
{
   va_list ap;
   va_start(ap, format);
   int status = vsnprintf(buffer, n, format, ap);
   va_end(ap);
   return status;
}
