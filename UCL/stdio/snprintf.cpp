//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdio.h"
#include "UCL/PrintF.h"

namespace {

class Buffer : public PrintF
{
public:
    Buffer(char* buffer_, size_t size_)
       : s(buffer_)
       , limit(buffer_ + size_ - 1)
    {
    }

    ~Buffer()
    {
       *s = '\0';
    }

private:
    void putc(char ch) override
    {
       if (s < limit)
          *s++ = ch;
       ++count;
    }

    char*       s;
    const char* limit;
};

} // namespace

int vsnprintf(char* buffer_, size_t n_, const char* format_, va_list ap_)
{
   Buffer buffer(buffer_, n_);
   return buffer.vprintf(format_, ap_);
}

int snprintf(char* buffer_, size_t n_, const char* format_, ...)
{
   Buffer buffer(buffer_, n_);
   va_list ap;
   va_start(ap, format_);
   int status = buffer.vprintf(format_, ap);
   va_end(ap);
   return status;
}
