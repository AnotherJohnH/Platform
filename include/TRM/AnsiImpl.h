//------------------------------------------------------------------------------
// Copyright (c) 2017-2018 John D. Haughton
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

#ifndef TRM_ANSI_IMPL_H
#define TRM_ANSI_IMPL_H

#include <cassert>
#include <cstdio>
#include <cstring>

#include "TRM/Ansi.h"

namespace TRM {

//! A partial implementation of TRM::Ansi
template <unsigned MAX_COLS, unsigned MAX_ROWS>
class AnsiImpl : public Ansi
{
protected:
   enum class Flash     : uint8_t { OFF, SLOW, FAST };
   enum class Intensity : uint8_t { NORMAL, BOLD, FAINT };
   enum class Cursor    : uint8_t { OFF, BLOCK, LINE};

   class Attr
   {
   private:
      uint8_t attr{};
      uint8_t bg{};
      uint8_t fg{};

      void pack(unsigned msb, unsigned lsb, unsigned value)
      {
         unsigned bits = msb - lsb + 1;
         uint16_t mask = ((1 << bits) - 1) << lsb;
         attr          = attr & ~mask;
         attr          = attr | ((value << lsb) & mask);
      }

      unsigned unpack(unsigned msb, unsigned lsb) const
      {
         unsigned bits = msb - lsb + 1;
         uint16_t mask = (1 << bits) - 1;
         return (attr >> lsb) & mask;
      }

   public:
      Attr() { reset(); }

      void reset()
      {
         setIntensity(Intensity::NORMAL);
         setItalic(false);
         setUnderline(false);
         setInvert(false);
         setFont(0);
         setFgCol(8);
         setBgCol(8);
         setFlash(Flash::OFF);
      }

      Intensity getIntensity() const { return Intensity(unpack( 1,  0)); }
      bool      isItalic()     const { return      bool(unpack( 2,  2)); }
      bool      isUnderline()  const { return      bool(unpack( 3,  3)); }
      bool      isInvert()     const { return      bool(unpack( 4,  4)); }
      unsigned  getFont()      const { return           unpack( 6,  5);  }
      unsigned  getFgCol()     const { return           fg;              }
      unsigned  getBgCol()     const { return           bg;              }
      Flash     getFlash()     const { return           unpack(15, 15) ? Flash::SLOW : Flash::OFF; }

      bool isBold()   const { return getIntensity() == Intensity::BOLD; }
      bool isNormal() const { return getIntensity() == Intensity::NORMAL; }
      bool isFaint()  const { return getIntensity() == Intensity::FAINT; }

      void setIntensity(Intensity intensity) { pack( 1,  0, unsigned(intensity)); }
      void setItalic(bool on)                { pack( 2,  2, on ? 1 : 0); }
      void setUnderline(bool on)             { pack( 3,  3, on ? 1 : 0); }
      void setInvert(bool on)                { pack( 4,  4, on ? 1 : 0); }
      void setFont(unsigned font)            { pack( 6,  5, font); }
      void setFlash(Flash flash)             { pack( 7,  7, flash != Flash::OFF ? 1 : 0); }
      void setFgCol(unsigned colour)         { fg = colour; }
      void setBgCol(unsigned colour)         { bg = colour; }

      static uint8_t rgbToCol256(uint8_t red, uint8_t grn, uint8_t blu)
      {
         red /= 51;
         grn /= 51;
         blu /= 51;
         return 16 + red * 36 + grn * 6 + blu;
      }

      void setRgbFgCol(uint8_t red, uint8_t grn, uint8_t blu) { fg = rgbToCol256(red, grn, blu); }

      void setRgbBgCol(uint8_t red, uint8_t grn, uint8_t blu) { bg = rgbToCol256(red, grn, blu); }
   };

   AnsiImpl()
   {
      memset(cell_char, 0, sizeof(cell_char));

      ansiReset();
   }

   //! Check if terminal echo is enabled
   bool isEchoEnabled() const { return echo; }

   //! Change state of terminal echo
   void setEcho(bool enable) { echo = enable; }

   //! Resize the terminal
   void resize(unsigned num_cols_, unsigned num_rows_)
   {
      num_cols   = num_cols_ > MAX_COLS ? MAX_COLS : num_cols_;
      num_rows   = num_rows_ > MAX_ROWS ? MAX_ROWS : num_rows_;
      top_margin = 1;
      btm_margin = num_rows;
   }

   //! Render a character in the terminal emulation
   virtual void renderChar(unsigned col, unsigned row, uint8_t ch, Attr at) = 0;

   //! Return a string from terminal emulation
   virtual void returnString(const char* s) = 0;

   void drawCursor(Cursor mode)
   {
      drawChar(col, row, mode);
   }

private:
   //! Return an integer as a string
   void returnInt(signed value)
   {
      char temp[32];
      sprintf(temp, "%d", value);
      returnString(temp);
   }

   //! CSI cursor movement
   void csiCursor(uint8_t cmd, unsigned n = 0, unsigned m = 0)
   {
      if(n == 0) n = 1;
      if(m == 0) m = 1;

      switch(cmd)
      {
      case 'A':           row -= n; break;
      case 'B':           row += n; break;
      case 'C': col += n;           break;
      case 'D': col -= n;           break;
      case 'E': col =  1; row += n; break;
      case 'F': col =  1; row -= n; break;
      case 'G': col =  n;           break;
      case 'H': col =  m; row =  n; break;

      default:
         assert(!"not a cursor command");
         break;
      }

      if(row < 1)
         row = 1;
      else if(row > signed(btm_margin))
         row = btm_margin;

      if(col < 1)
         col = 1;
      else if(col > signed(num_cols))
         col = num_cols;
   }

   //! Clear part of the display
   void csiErase(uint8_t cmd, unsigned n)
   {
      if(n > 3) return;

      int init_col = col;
      int init_row = row;

      if(n != 0)
      {
         col = 1;
         if(cmd == 'J') row = 1;
      }

      int last_col = col;
      int last_row = row;

      if(n != 1)
      {
         last_col = num_cols;
         if(cmd == 'J') last_row = num_rows;
      }

      while((row != last_row) || (col != last_col))
      {
         ansiGraphic(' ');
      }

      ansiGraphic(' ');

      col = init_col;
      row = init_row;

      implicit_cr = false;
   }

   //! Select Graphic Rendition
   void csiSGR(unsigned n)
   {
      switch(sgr_state)
      {
      case 10:
      case 20:
         if(n == 5)
         {
            sgr_state += 1;
         }
         else if(n == 2)
         {
            sgr_state += 2;
         }
         else
         {
            break;
         }
         return;

      case 11:
         attr.setFgCol(n);
         sgr_state = 0;
         return;

      case 21:
         attr.setBgCol(n);
         sgr_state = 0;
         return;

      case 12:
      case 22:
         sgr_state_red = n;
         sgr_state++;
         return;

      case 13:
      case 23:
         sgr_state_grn = n;
         sgr_state++;
         return;

      case 14:
         attr.setRgbFgCol(sgr_state_red, sgr_state_grn, n);
         sgr_state = 0;
         return;

      case 24:
         attr.setRgbBgCol(sgr_state_red, sgr_state_grn, n);
         sgr_state = 0;
         return;

      case 0:
      default:
         break;
      }

      sgr_state = 0;

      switch(n)
      {
      case  0: attr.reset();              break;

      case 22: attr.setIntensity(Intensity::NORMAL); break;
      case  1: attr.setIntensity(Intensity::BOLD);   break;
      case  2: attr.setIntensity(Intensity::FAINT);  break;

      case 23: attr.setItalic(false);     break;
      case  3: attr.setItalic(true);      break;

      case 24: attr.setUnderline(false);  break;
      case  4: attr.setUnderline(true);   break;

      case 25: attr.setFlash(Flash::OFF);  break;
      case  5: attr.setFlash(Flash::SLOW); break;
      case  6: attr.setFlash(Flash::FAST); break;

      case 27: attr.setInvert(false);     break;
      case  7: attr.setInvert(true);      break;

      case 39: attr.setFgCol(8);          break;
      case 49: attr.setBgCol(8);          break;

      case 38: sgr_state = 10;            break;
      case 48: sgr_state = 20;            break;

      default:
         if((n >= 10) && (n <= 19))
         {
            attr.setFont(n - 10);
         }
         else if((n >= 30) && (n <= 37))
         {
            attr.setFgCol(n - 30);
         }
         else if((n >= 40) && (n <= 47))
         {
            attr.setBgCol(n - 40);
         }
         break;
      }
   }

   //! Device Status Report
   void csiDSR(unsigned n)
   {
      switch(n)
      {
      case 5: // Status report => ok
         returnString("\033[0n");
         break;

      case 6: // Cursor position report
         returnString("\033[");
         returnInt(row);
         returnString(";");
         returnInt(col);
         returnString("R");
         break;

      default:
         break;
      }
   }

   //! Save Cursor Position
   void csiSCP()
   {
      save_col = col;
      save_row = row;
   }

   //! Restore Cursor Position
   void csiRCP()
   {
      col = save_col;
      row = save_row;
   }

   void drawChar(unsigned c, unsigned r, Cursor cursor_mode = Cursor::OFF)
   {
      Attr at = cell_attr[c - 1][r - 1];

      switch(cursor_mode)
      {
      case Cursor::OFF: break;
      case Cursor::LINE: at.setUnderline(!at.isUnderline()); break;
      case Cursor::BLOCK: at.setInvert(!at.isInvert()); break;
      }

      renderChar(c, r, cell_char[c - 1][r - 1], at);
   }

   void scroll()
   {
      for(unsigned r = top_margin; r < btm_margin; ++r)
      {
         for(unsigned c = 1; c <= num_cols; ++c)
         {
            cell_char[c - 1][r - 1] = cell_char[c - 1][r];
            cell_attr[c - 1][r - 1] = cell_attr[c - 1][r];

            drawChar(c, r);
         }
      }

      for(unsigned c = 1; c <= num_cols; ++c)
      {
         cell_char[c - 1][btm_margin - 1] = ' ';
         cell_attr[c - 1][btm_margin - 1].reset();

         drawChar(c, btm_margin);
      }
   }

   void nextLine()
   {
      col = 1;
      if(++row > signed(btm_margin))
      {
         row = btm_margin;

         scroll();
      }
   }

   // implement Ansi

   virtual void ansiReset() override
   {
      col = 1;
      row = 1;
      attr.reset();
      echo        = true;
      implicit_cr = false;
   }

   virtual void ansiGraphic(uint8_t ch) override
   {
      cell_char[col - 1][row - 1] = ch;
      cell_attr[col - 1][row - 1] = attr;

      drawChar(col, row);

      implicit_cr = (col == signed(num_cols));
      if(implicit_cr)
      {
         nextLine();
      }
      else
      {
         csiCursor('C');
      }
   }

   virtual void ansiControl(uint8_t ch) override
   {
      switch(ch)
      {
      case '\b':
         csiCursor('D');
         break;

      case '\n':
         if(implicit_cr)
         {
            implicit_cr = false;
         }
         else
         {
            nextLine();
         }
         break;

      case '\f':
         csiErase('J', 2);
         break;

      default:
         break;
      }
   }

   virtual void ansiCsi(uint8_t cmd, const char* seq) override
   {
      unsigned n = 0;
      unsigned m = 0;

      switch(cmd)
      {
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
      case 'G':
         parseUInt(seq, n);
         csiCursor(cmd, n);
         break;

      case 'H':
      case 'f':
         parseUInt(seq, n);
         if(*seq == ';')
         {
            parseUInt(++seq, m);
         }
         csiCursor('H', n, m);
         break;

      case 'J':
      case 'K':
         parseUInt(seq, n);
         csiErase(cmd, n);
         break;

      case 'i':
         parseUInt(seq, n);
         // csiAUX(n);
         break;

      case 'm':
         parseUInt(seq, n);
         csiSGR(n);
         while(*seq == ';')
         {
            n = 0;
            parseUInt(++seq, n);
            csiSGR(n);
         }
         break;

      case 'n':
         parseUInt(seq, n);
         csiDSR(n);
         break;

      case 's':
         csiSCP();
         break;

      case 'r': // not strictly ANSI
         top_margin = 1;
         parseUInt(seq, top_margin);
         btm_margin = num_rows;
         if(*seq == ';')
         {
            parseUInt(++seq, btm_margin);
         }
         btm_margin -= 1;
         break;

      case 'u':
         csiRCP();
         break;

      default: break;
      }
   }

   unsigned num_cols{};
   unsigned num_rows{};
   signed   col{1}, row{1};
   signed   save_col{}, save_row{};
   Attr     attr;
   bool     echo{};
   Attr     cell_attr[MAX_COLS][MAX_ROWS];
   uint8_t  cell_char[MAX_COLS][MAX_ROWS];
   bool     implicit_cr{};
   uint8_t  sgr_state{0};
   uint8_t  sgr_state_red{0};
   uint8_t  sgr_state_grn{0};
   unsigned top_margin{1};
   unsigned btm_margin{};
};


} // namespace TRM

#endif // TRM_ANSI_IMPL_H
