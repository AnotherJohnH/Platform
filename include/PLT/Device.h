//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef PLT_DEVICE_H
#define PLT_DEVICE_H

#include <cstdlib>

namespace PLT {

class Device
{
public:

#define IOCTL_TERM(N) (0x100|N)

   static const unsigned long IOCTL_TERM_ECHO       = IOCTL_TERM(0);
   static const unsigned long IOCTL_TERM_ICANON     = IOCTL_TERM(2);
   static const unsigned long IOCTL_TERM_PALETTE    = IOCTL_TERM(3);
   static const unsigned long IOCTL_TERM_BORDER     = IOCTL_TERM(4);
   static const unsigned long IOCTL_TERM_LINE_SPACE = IOCTL_TERM(5);
   static const unsigned long IOCTL_TERM_FONT_SIZE  = IOCTL_TERM(6);
   static const unsigned long IOCTL_TERM_TIMEOUT_MS = IOCTL_TERM(7);
   static const unsigned long IOCTL_TERM_COLOURS    = IOCTL_TERM(8);
   static const unsigned long IOCTL_TERM_FONTS      = IOCTL_TERM(9);

   virtual int open(unsigned oflag) { return 0; }

   virtual int ioctl(unsigned request, ...) { return -1; }

   virtual int write(const void* buffer, size_t n) { return 0; }

   virtual int read(void* buffer_, size_t n) { return -1; }

   virtual int close() { return 0; }
};

} // namespace PLT

#endif
