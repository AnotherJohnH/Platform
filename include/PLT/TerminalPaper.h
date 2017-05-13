//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

#ifndef PLT_TERMINAL_PAPER_H
#define PLT_TERMINAL_PAPER_H

#include <cassert>

#include "PLT/Paper.h"
#include "PLT/Event.h"
#include "PLT/Ansi.h"
#include "PLT/Device.h"

#include "GUI/Font/Teletext9.h"
#include "GUI/Font/Teletext12.h"
#include "GUI/Font/Teletext15.h"
#include "GUI/Font/Teletext18.h"

#include "STB/Fifo.h"

namespace PLT {

//! Terminal device using PLT::Paper back-end
template <unsigned WIDTH, unsigned HEIGHT>
class TerminalPaper : public Ansi
                    , public Device
{
private:
   static const unsigned MIN_FONT_WIDTH  = 6;
   static const unsigned MIN_FONT_HEIGHT = 8;
   static const unsigned MAX_COLS = WIDTH  / MIN_FONT_WIDTH;
   static const unsigned MAX_ROWS = HEIGHT / MIN_FONT_HEIGHT;

   enum Flash     { OFF, SLOW, FAST };
   enum Intensity { NORMAL, BOLD, FAINT };

   class Attr
   {
   private:
      uint16_t attr{};

      void pack(unsigned msb, unsigned lsb, unsigned value)
      {
         unsigned bits = msb - lsb + 1;
         uint16_t mask = ((1<<bits) - 1)<<lsb;
         attr = attr & ~mask;
         attr = attr | ((value<<lsb) & mask);
      }

      unsigned unpack(unsigned msb, unsigned lsb) const
      {
         unsigned bits = msb - lsb + 1;
         uint16_t mask = (1<<bits) - 1;
         return (attr >> lsb) & mask;
      }

   public:
      Attr()
      {
         reset();
      }

      void reset()
      {
         setIntensity(NORMAL);
         setItalic(false);
         setUnderline(false);
         setInvert(false);
         setFont(0);
         setFgCol(7);
         setBgCol(0);
         setFlash(OFF);
      }

      Intensity  getIntensity() const { return Intensity(unpack( 1,  0)); }
      bool       isItalic()     const { return      bool(unpack( 2,  2)); }
      bool       isUnderline()  const { return      bool(unpack( 3,  3)); }
      bool       isInvert()     const { return      bool(unpack( 4,  4)); }
      unsigned   getFont()      const { return           unpack( 7,  5);  }
      unsigned   getFgCol()     const { return           unpack(10,  8);  }
      unsigned   getBgCol()     const { return           unpack(13, 11);  }
      Flash      getFlash()     const { return     Block(unpack(15, 14)); }

      bool       isBold()       const { return getIntensity() == BOLD;   }
      bool       isNormal()     const { return getIntensity() == NORMAL; }
      bool       isFaint()      const { return getIntensity() == FAINT;  }

      void  setIntensity(Intensity intensity) { pack( 1,  0, unsigned(intensity)); }
      void  setItalic(bool on)                { pack( 2,  2, on ? 1 : 0); }
      void  setUnderline(bool on)             { pack( 3,  3, on ? 1 : 0); }
      void  setInvert(bool on)                { pack( 4,  4, on ? 1 : 0); }
      void  setFont(unsigned font)            { pack( 7,  5, font); }
      void  setFgCol(unsigned col)            { pack(10,  8, col); }
      void  setBgCol(unsigned col)            { pack(13, 11, col); }
      void  setFlash(Flash flash)             { pack(15, 14, unsigned(flash)); }
   };

   // Resources
   Paper             paper;
   const GUI::Font*  font{};
   unsigned          border{0};
   unsigned          line_space{0};
   unsigned          num_cols{};
   unsigned          num_rows{};
   GUI::Vector       org;

   // State
   GUI::Colour           palette[8];
   signed                col{}, row{};
   signed                save_col{}, save_row{};
   Attr                  attr;
   bool                  echo{};
   Attr                  cell_attr[MAX_COLS][MAX_ROWS];
   uint8_t               cell_char[MAX_COLS][MAX_ROWS];
   STB::Fifo<uint8_t,6>  response;
   bool                  implicit_cr{};

   //! CSI cursor movement
   void csiCursor(uint8_t cmd, unsigned n=0, unsigned m=0)
   {
      if (n == 0) n = 1;
      if (m == 0) m = 1;

      switch(cmd)
      {
      case 'A':           row -= n; break;
      case 'B':           row += n; break;
      case 'C': col += n;           break;
      case 'D': col -= n;           break;
      case 'E': col =  1; row += n; break;
      case 'F': col =  1; row -= n; break;
      case 'G': col =  n;            break;
      case 'H': col =  m; row =  n;  break;

      default:
         assert(!"not a cursor command");
         break;
      }

      if (row < 1)
         row = 1;
      else if (row > signed(num_rows))
         row = num_rows;

      if (col < 1)
         col = 1;
      else if (col > signed(num_cols))
         col = num_cols;
   }

   //! Clear part of the display
   void csiErase(uint8_t cmd, unsigned n)
   {
      if (n > 3) return;

      int init_col = col;
      int init_row = row;

      if (n != 0)
      {
         col = 1;
         if (cmd == 'J') row = 1;
      }

      int last_col = col;
      int last_row = row;

      if (n != 1)
      {
         last_col = num_cols;
         if (cmd == 'J') last_row = num_rows;
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
      switch(n)
      {
      case  0: attr.reset();              break;

      case 22: attr.setIntensity(NORMAL); break;
      case  1: attr.setIntensity(BOLD);   break;
      case  2: attr.setIntensity(FAINT);  break;

      case 23: attr.setItalic(false);     break;
      case  3: attr.setItalic(true);      break;

      case 24: attr.setUnderline(false);  break;
      case  4: attr.setUnderline(true);   break;

      case 25: attr.setFlash(OFF);        break;
      case  5: attr.setFlash(SLOW);       break;
      case  6: attr.setFlash(FAST);       break;

      case 27: attr.setInvert(false);     break;
      case  7: attr.setInvert(true);      break;

      case 39: attr.setFgCol(7);          break;
      case 49: attr.setBgCol(0);          break;

      default:
         if ((n >= 10) && (n <= 19))
         {
            attr.setFont(n - 10);;
         }
         else if ((n >= 30) && (n <= 37))
         {
            attr.setFgCol(n - 30);;
         }
         else if ((n >= 40) && (n <= 47))
         {
            attr.setBgCol(n - 40);;
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
         sendString("\033[0n");
         break;

      case 6:
         sendString("\033[");
         rprintf("%d", row);
         sendString(";");
         rprintf("%d", col);
         sendString("R");
         break; // Cursor position report

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

   void scroll()
   {
      for(unsigned r=1; r<num_rows; ++r)
      {
         for(unsigned c=1; c<=num_cols; ++c)
         {
            cell_char[c-1][r-1] = cell_char[c-1][r];
            cell_attr[c-1][r-1] = cell_attr[c-1][r];

            drawChar(c, r);
         }
      }

      for(unsigned c=1; c<=num_cols; ++c)
      {
         cell_char[c-1][num_rows-1] = ' ';
         cell_attr[c-1][num_rows-1] = attr;

         drawChar(c, num_rows);
      }
   }

   void nextLine()
   {
      col = 1;
      if (++row > signed(num_rows))
      {
         row = num_rows;

         scroll();
      }
   }

   void drawChar(unsigned c, unsigned r)
   {
      uint8_t ch = cell_char[c-1][r-1];
      Attr    at = cell_attr[c-1][r-1];

      unsigned fg = at.getFgCol();
      unsigned bg = at.getBgCol();

      unsigned x = org.x + (c-1) * font->getWidth();
      unsigned y = org.y + (r-1) * (font->getHeight() + line_space);

      paper.fillRect(palette[attr.isInvert() ? fg : bg],
                     x, y,
                     x + font->getWidth(),
                     y + font->getHeight() + line_space);

      paper.drawChar(palette[at.isInvert() ? bg : fg],
                     palette[at.isInvert() ? fg : bg],
                     x, y + line_space, font, ch);

      if (at.isBold())
      {
         paper.drawChar(palette[at.isInvert() ? bg : fg],
                        palette[at.isInvert() ? fg : bg],
                        x + 1, y + line_space, font, ch);
      }

      if (at.isUnderline())
      {
         paper.drawLine(palette[at.isInvert() ? bg : fg],
                        x,                    y + font->getHeight() - 1,
                        x + font->getWidth(), y + font->getHeight() - 1);
      }
   }

   // implement Ansi

   virtual void ansiReset() override
   {
      col = 1;
      row = 1;
      attr.reset();
      echo = true;
      implicit_cr = false;

      palette[0] = GUI::BLACK;
      palette[1] = GUI::RED;
      palette[2] = GUI::GREEN;
      palette[3] = GUI::YELLOW;
      palette[4] = GUI::BLUE;
      palette[5] = GUI::MAGENTA;
      palette[6] = GUI::CYAN;
      palette[7] = GUI::WHITE;
   }

   virtual void ansiGraphic(uint8_t ch) override
   {
      cell_char[col-1][row-1] = ch;
      cell_attr[col-1][row-1] = attr;

      drawChar(col, row);

      implicit_cr = (col == signed(num_cols));
      if (implicit_cr)
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
         if (implicit_cr)
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
         if (*seq == ';')
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

      case 'r':
         csiRCP();
         break;

      default: break;
      }
   }

   void sendString(const char* s)
   {
      while(*s)
      {
         assert(!response.full());
         uint8_t ch = *s++;
         response.push(ch);
      }
   }

   void rprintf(const char* format, ...)
   {
      char  temp[32];

      va_list  ap;
      va_start(ap, format);
      vsprintf(temp, format, ap);
      va_end(ap);

      sendString(temp);
   }

   bool getInput(uint8_t& ch)
   {
      if (!response.empty())
      {
         ch = response.back();
         response.pop();
         return true;
      }

      while(true)
      {
         PLT::Event  event;

         switch(PLT::waitEvent(event))
         {
         case PLT::QUIT:
            return false;

         case PLT::KEY_DOWN:
            if (echo && (event.code < 0x80))
            {
               ansiWrite(event.code);
            }
            ch = event.code;
            return true;

         default:
            break;
         }
      }
   }

   void init()
   {
      num_cols = (WIDTH - 2*border) / font->getWidth();
      if (num_cols > MAX_COLS) num_cols = MAX_COLS;

      num_rows = (HEIGHT - 2*border) / (font->getHeight() + line_space);
      if (num_rows > MAX_ROWS) num_rows = MAX_ROWS;

      org.x = (WIDTH  - (num_cols * font->getWidth()))/2;
      org.y = border;

      paper.clear(palette[0]);
   }

public:
   TerminalPaper(const char* title_)
      : paper(title_, WIDTH, HEIGHT)
      , font(&GUI::font_teletext15)
      , border(0)
      , line_space(0)
   {
      init();
      ansiReset();
   }

   ~TerminalPaper()
   {
      paper.refresh();
   }

   void setFont(const GUI::Font& font_)
   {
      font = &font_;
      init();
   }

   // implement PLT::Device

   virtual int ioctl(unsigned request, ...) override
   {
      int     status = -1;
      va_list ap;

      va_start(ap, request);

      switch(request)
      {
      case IOCTL_TERM_ICANON:
         (void) va_arg(ap, int);
         status = 0;
         break;

      case IOCTL_TERM_ECHO:
         echo = va_arg(ap, int);
         status = 0;
         break;

      case IOCTL_TERM_PALETTE:
         {
            unsigned col = va_arg(ap, unsigned);
            palette[col] = va_arg(ap, uint32_t);
            init();
            status = 0;
         }
         break;

      case IOCTL_TERM_BORDER:
         border = va_arg(ap, unsigned);
         init();
         status = 0;
         break;

      case IOCTL_TERM_LINE_SPACE:
         line_space = va_arg(ap, unsigned);
         init();
         status = 0;
         break;

      case IOCTL_TERM_FONT_SIZE:
         {
            unsigned font_size = va_arg(ap, unsigned);

            if      (font_size >= 18)  { setFont(GUI::font_teletext18); }
            else if (font_size >= 15)  { setFont(GUI::font_teletext15); }
            else if (font_size >= 12 ) { setFont(GUI::font_teletext12); }
            else                       { setFont(GUI::font_teletext9);  }

            init();
            status = 0;
         }
         break;

      default:
         break;
      }

      va_end(ap);

      return status;
   }

   virtual ssize_t write(const void* buffer, size_t n) override
   {
      const uint8_t* ptr = static_cast<const uint8_t*>(buffer);

      while(n--)
      {
         ansiWrite(*ptr++);
      }

      return n;
   }

   virtual ssize_t read(void* buffer_, size_t n) override
   {
      uint8_t* buffer = static_cast<uint8_t*>(buffer_);

      size_t i;

      for(i = 0; i < n; i++)
      {
         paper.refresh();

         uint8_t ch;

         if (!getInput(ch)) return -1;

         buffer[i] = ch;
      }

      return i;
   }
};

} // namespace PLT

#endif
