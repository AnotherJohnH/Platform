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

#ifndef Z_WINDOW_H
#define Z_WINDOW_H

#include "ZLog.h"

#include "ZStream.h"

//! Interface to console implementation
class ZWindowManager : public ZStream
{
private:
   static const unsigned MAX_WINDOW = 8;

   struct Vec
   {
      uint16_t x, y;

      Vec() : x(0), y(0) {}
   };

   struct ZWindow
   {
      Vec       pos;
      Vec       size;
      Vec       cursor;
      uint16_t  left_margin;
      uint16_t  right_margin;
      uint16_t  newline_handler;
      uint16_t  interrupt_countdown;
      uint8_t   text_style;
      uint16_t  colour_data;
      uint8_t   font_number;
      uint8_t   font_size;
      uint8_t   attr;
      int16_t   line_count;

      ZWindow()
         : left_margin(0)
         , right_margin(0)
         , newline_handler(0)
         , interrupt_countdown(0)
         , text_style(0)
         , colour_data(0)
         , font_number(0)
         , font_size(0)
         , attr(0)
         , line_count(0)
      {}
   };

public:
   enum Window
   {
      LOWER = 0,
      UPPER = 1
   };

   ZWindowManager(PLT::Device* device, ZMemory& memory)
      : ZStream(device, memory)
      , index(LOWER)
      , lower_buffering(true)
   {
      enableStream(2, true); // Printer
   }

   uint16_t getWindowProp(unsigned index_, unsigned prop_)
   {
      switch(prop_)
      {
      case  0: return window[index_].pos.y;
      case  1: return window[index_].pos.x;
      case  2: return window[index_].size.y;
      case  3: return window[index_].size.x;
      case  4: return window[index_].cursor.y;
      case  5: return window[index_].cursor.x;
      case  6: return window[index_].left_margin;
      case  7: return window[index_].right_margin;
      case  8: return window[index_].newline_handler;
      case  9: return window[index_].interrupt_countdown;
      case 10: return window[index_].text_style;
      case 11: return window[index_].colour_data;
      case 12: return window[index_].font_number;
      case 13: return window[index_].font_size;
      case 14: return window[index_].attr;
      case 15: return window[index_].line_count;

      default: return 0;
      }
   }

   // Update the status line (v1-3)
   void showStatus(const char* left, const char* right)
   {
      enableStream(2, false); // Printer

      move(1, 1);

      for(unsigned i=0; i<cols; i++)
      {
         if (left[i] == '\0') break;

         addch(left[i]);
      }

      move(1, 999);

      // TODO this needs sorting out

      enableStream(2, true); // Printer
   }

   void split(unsigned upper_height_)
   {
      window[UPPER].size.y = upper_height_;
   }

   void select(unsigned index_)
   {
      if (index == index_) return;

      index = index_;

      if (index == UPPER)
      {
         enableStream(2, false); // Printer

         unsigned line, col;
         getyx(line, col);
         window[LOWER].cursor.y = line;
         window[LOWER].cursor.x = col;
         lower_buffering = setBuffering(false);
         setCol(1);
         move(1, 1);
      }
      else
      {
         enableStream(2, true); // Printer

         move(window[LOWER].cursor.y, window[LOWER].cursor.x);
         setBuffering(lower_buffering);
         setCol(window[LOWER].cursor.x);
      }
   }

   void eraseWindow(unsigned index_)
   {
      // TODO
   }

private:
   ZWindow    window[MAX_WINDOW];
   unsigned   index;
   bool       lower_buffering;
};

#endif
