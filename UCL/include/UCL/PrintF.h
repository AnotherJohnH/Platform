//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <stdarg.h>

class PrintF
{
public:
   PrintF() = default;

   virtual ~PrintF() = default;

   //! Write a character to the output stream
   virtual void putc(char ch) = 0;

   //! Perform a formatted print
   int vprintf(const char* format, va_list ap);

   //! Return number of characters that wer attempted to be written
   unsigned size() const { return count; }

protected:
   unsigned count{0};
};
