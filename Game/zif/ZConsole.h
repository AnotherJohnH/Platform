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

#ifndef Z_CONSOLE_H
#define Z_CONSOLE_H

#include <cstdint>

#include "PLT/Curses.h"
#include "PLT/Device.h"

//! Console interface
class ZConsole : public PLT::Curses
{
public:
   enum Attr
   {
      LINES,
      COLS,

      COLOURS,
      BOLD,
      ITALIC,

      FONT_HEIGHT,
      FONT_WIDTH,

      PICTURE_FONT,
      GRAPHIC_FONT,
      FIXED_FONT,

      READ_TIMEOUT
   };

   ZConsole(PLT::Device* dev_);

   ~ZConsole();

   //! Return console attribute
   unsigned getAttr(Attr attr) const
   {
      switch(attr)
      {
      case LINES:        return lines;
      case COLS:         return cols;

      case COLOURS:      return true;   // TODO platform specific
      case BOLD:         return true;
      case ITALIC:       return false;

      case FONT_HEIGHT:  return 10;     // TODO this value is fake
      case FONT_WIDTH:   return 6;      // TODO this value is fake

      case PICTURE_FONT: return false;
      case GRAPHIC_FONT: return false;
      case FIXED_FONT:   return true;

      case READ_TIMEOUT: return false;  // TODO

      default: return 0;
      }
   }

   //! Set current font
   void setFont(unsigned font_idx)
   {
      // TODO
   }

   //! Write ZSCII character to printer
   void print(uint16_t zscii);

   //! Snoop input ZSCII character
   void snoop(uint16_t zscii);

   //! Read ZSCII character
   bool read(uint16_t& zscii, unsigned timeout);
};

#endif
