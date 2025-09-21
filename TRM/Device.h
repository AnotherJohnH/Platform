//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdlib>
#include <cstring>

namespace TRM {

static constexpr unsigned long IOCTL_TERM(unsigned n) { return 0x100 | n; }

//! Terminal device API
class Device
{
public:
   static const unsigned long IOCTL_TERM_ECHO        = IOCTL_TERM(0);
   static const unsigned long IOCTL_TERM_ICANON      = IOCTL_TERM(2);
   static const unsigned long IOCTL_TERM_PALETTE     = IOCTL_TERM(3);
   static const unsigned long IOCTL_TERM_BORDER      = IOCTL_TERM(4);
   static const unsigned long IOCTL_TERM_LINE_SPACE  = IOCTL_TERM(5);
   static const unsigned long IOCTL_TERM_FONT_SIZE   = IOCTL_TERM(6);
   static const unsigned long IOCTL_TERM_TIMEOUT_MS  = IOCTL_TERM(7);
   static const unsigned long IOCTL_TERM_COLOURS     = IOCTL_TERM(8);
   static const unsigned long IOCTL_TERM_FONTS       = IOCTL_TERM(9);
   static const unsigned long IOCTL_TERM_CURSOR      = IOCTL_TERM(10);
   static const unsigned long IOCTL_TERM_SLEEP       = IOCTL_TERM(11);
   static const unsigned long IOCTL_TERM_SLEEP_IMAGE = IOCTL_TERM(12);

   virtual int open(unsigned oflag) { return 0; }

   virtual int ioctl(unsigned request, ...) { return -1; }

   //! Write a block of data to the device
   virtual int write(const void* buffer, size_t n) { return 0; }

   //! Read a block of data from the device
   virtual int read(void* buffer_, size_t n) { return -1; }

   virtual int close() { return 0; }

   //! Write a single character to the device
   int write(char ch)       { return write(&ch, 1); }

   //! Write a null terminated string to the device
   int write(const char* s) { return write(s, strlen(s)); }

   //! Read a single character from the device
   int read(char& ch)       { return read(&ch, 1); }
};

} // namesapce TRM
