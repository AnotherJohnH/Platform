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

#ifndef TRM_FRAME_H
#define TRM_FRAME_H

#include <cassert>
#include <cstdarg>

#include "STB/Fifo.h"

#include "PLT/Event.h"

#include "GUI/Font/Teletext.h"
#include "GUI/Frame.h"

#include "TRM/AnsiImpl.h"
#include "TRM/Device.h"

namespace TRM {


static const unsigned MIN_FONT_WIDTH  = 6;
static const unsigned MIN_FONT_HEIGHT = 8;


//! Terminal device using GUI::Frame back-end
template <unsigned WIDTH, unsigned HEIGHT>
class Frame
   : public Device
   , private AnsiImpl<WIDTH/MIN_FONT_WIDTH, HEIGHT/MIN_FONT_HEIGHT>
{
private:
   using Impl = AnsiImpl<WIDTH / MIN_FONT_WIDTH, HEIGHT / MIN_FONT_HEIGHT>;

   static const unsigned DEFAULT_BG_COL = 0;
   static const unsigned DEFAULT_FG_COL = 1;
   static const uint8_t  RGB_NRM        = 0xC0;

   GUI::Frame            frame;
   const GUI::Font*      font{};
   unsigned              border{0};
   unsigned              line_space{0};
   GUI::Vector           org;
   STB::Colour           default_bg_col{STB::BLACK};
   STB::Colour           default_fg_col{STB::WHITE};
   bool                  draw_cursor{true};
   unsigned              timeout_ms{0};
   STB::Fifo<uint8_t, 6> response;
   bool                  shift{false};

   STB::Colour convertCol256ToRGB(uint8_t col, bool bg)
   {
      switch(col)
      {
      case  0: return STB::BLACK;
      case  1: return STB::RGB(RGB_NRM,       0,       0);
      case  2: return STB::RGB(      0, RGB_NRM,       0);
      case  3: return STB::RGB(RGB_NRM, RGB_NRM,       0);
      case  4: return STB::RGB(      0,       0, RGB_NRM);
      case  5: return STB::RGB(RGB_NRM,       0, RGB_NRM);
      case  6: return STB::RGB(      0, RGB_NRM, RGB_NRM);
      case  7: return STB::RGB(RGB_NRM, RGB_NRM, RGB_NRM);
      case  8: return bg ? default_bg_col : default_fg_col;
      case  9: return STB::RED;
      case 10: return STB::GREEN;
      case 11: return STB::YELLOW;
      case 12: return STB::BLUE;
      case 13: return STB::MAGENTA;
      case 14: return STB::CYAN;
      case 15: return STB::WHITE;

      default:
         if(col < 232)
         {
            col -= 16;
            uint8_t blu = (col % 6) * 51;
            col /= 6;
            uint8_t grn = (col % 6) * 51;
            col /= 6;
            uint8_t red = (col % 6) * 51;
            return STB::RGB(red, blu, grn);
         }
         else
         {
            col -= 232;
            uint8_t lvl = col * 11;
            return STB::GREY(lvl);
         }
      }
   }

   // Implement AnsiImpl

   virtual void renderChar(unsigned            c,
                           unsigned            r,
                           uint8_t             ch,
                           typename Impl::Attr at) override
   {
      STB::Colour fg, bg;

      if(at.isInvert())
      {
         fg = convertCol256ToRGB(at.getBgCol(), /* bg */ true);
         bg = convertCol256ToRGB(at.getFgCol(), /* bg */ false);
      }
      else
      {
         fg = convertCol256ToRGB(at.getFgCol(), /* bg */ false);
         bg = convertCol256ToRGB(at.getBgCol(), /* bg */ true);
      }

      unsigned x = org.x + (c - 1) * font->getWidth();
      unsigned y = org.y + (r - 1) * (font->getHeight() + line_space);

      frame.fillRect(bg, x, y, x + font->getWidth(), y + font->getHeight() + line_space);

      frame.drawChar(fg, bg, x, y + line_space, font, ch);

      if(at.isBold())
      {
         frame.drawChar(fg, bg, x + 1, y + line_space, font, ch);
      }

      // TODO use an italic font for italics
      if(at.isUnderline() || at.isItalic())
      {
         frame.drawLine(fg, x, y + font->getHeight() - 1, x + font->getWidth(),
                        y + font->getHeight() - 1);
      }
   }

   virtual void returnString(const char* s) override
   {
      while(*s)
      {
         assert(!response.full());
         uint8_t ch = *s++;
         response.push(ch);
      }
   }

   uint8_t shiftChar(uint8_t ch)
   {
      if ((ch >= 'a') && (ch <= 'z'))
      {
         return ch - 'a' + 'A';
      }

      // TODO configurable to actual keyboard
      switch(ch)
      {
      case '1': return '!';
      case '2': return '@';
      case '3': return '#';
      case '4': return '$';
      case '5': return '%';
      case '6': return '^';
      case '7': return '&';
      case '8': return '*';
      case '9': return '(';
      case '0': return ')';
      case '-': return '_';
      case '=': return '+';
      case '[': return '{';
      case ']': return '}';
      case ';': return ':';
      case '\'': return '"';
      case '\\': return '|';
      case '`': return '~';
      case ',': return '<';
      case '.': return '>';
      case '/': return '?';
      }

      return ch;
   }

   int getInput(uint8_t& ch)
   {
      if(!response.empty())
      {
         ch = response.back();
         response.pop();
         return 1;
      }

      int status;

      if(timeout_ms != 0)
      {
         PLT::Event::setTimer(timeout_ms);
      }

      while(true)
      {
         PLT::Event::Message event;
         PLT::Event::Type    type = PLT::Event::wait(event);

         if(type == PLT::Event::QUIT)
         {
            status = -1;
            break;
         }
         else if(type == PLT::Event::TIMER)
         {
            status = 0;
            break;
         }
         else if(type == PLT::Event::KEY_DOWN)
         {
            if((event.code == PLT::KeyCode::LSHIFT) ||
               (event.code == PLT::KeyCode::RSHIFT))
            {
               shift = true;
            }
            else
            {
               if (shift)
               {
                  event.code = shiftChar(event.code);
               }

               if(this->isEchoEnabled() && (event.code < 0x80))
               {
                  this->ansiWrite(event.code);
               }
               ch     = event.code;
               status = 1;
               break;
            }
         }
         else if(type == PLT::Event::KEY_UP)
         {
            if((event.code == PLT::KeyCode::LSHIFT) ||
               (event.code == PLT::KeyCode::RSHIFT))
            {
               shift = false;
            }
         }
      }

      if(timeout_ms != 0)
      {
         PLT::Event::setTimer(0);
      }

      return status;
   }

   void initLayout()
   {
      unsigned cols = (WIDTH - 2 * border) / font->getWidth();
      unsigned rows = (HEIGHT - 2 * border) / (font->getHeight() + line_space);

      this->resize(cols, rows);

      org.x = (WIDTH - (cols * font->getWidth())) / 2;
      org.y = border;

      frame.clear(default_bg_col);
   }

public:
   Frame(const char* title_)
      : frame(title_, WIDTH, HEIGHT)
      , font(&GUI::font_teletext15)
      , border(0)
      , line_space(0)
   {
      initLayout();
   }

   ~Frame() { frame.refresh(); }

   void setFont(const GUI::Font& font_)
   {
      font = &font_;
      initLayout();
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
         (void)va_arg(ap, int);
         status = 0;
         break;

      case IOCTL_TERM_ECHO:
         this->setEcho(va_arg(ap, int) != 0);
         status = 0;
         break;

      case IOCTL_TERM_PALETTE:
      {
         unsigned    col = va_arg(ap, unsigned);
         STB::Colour rgb = va_arg(ap, unsigned);
              if (col == DEFAULT_BG_COL) { default_bg_col = rgb; }
         else if (col == DEFAULT_FG_COL) { default_fg_col = rgb; }
         initLayout();
         status = 0;
      }
      break;

      case IOCTL_TERM_BORDER:
         border = va_arg(ap, unsigned);
         initLayout();
         status = 0;
         break;

      case IOCTL_TERM_LINE_SPACE:
         line_space = va_arg(ap, unsigned);
         initLayout();
         status = 0;
         break;

      case IOCTL_TERM_FONT_SIZE:
      {
         unsigned font_size = va_arg(ap, unsigned);

              if (font_size >= 18)  { setFont(GUI::font_teletext18); }
         else if (font_size >= 15)  { setFont(GUI::font_teletext15); }
         else if (font_size >= 12)  { setFont(GUI::font_teletext12); }
         else                       { setFont(GUI::font_teletext9);  }

         initLayout();
         status = 0;
      }
      break;

      case IOCTL_TERM_TIMEOUT_MS:
         timeout_ms = va_arg(ap, unsigned);
         status     = 0;
         break;

      case IOCTL_TERM_COLOURS:
         status = 256;
         break;

      case IOCTL_TERM_FONTS:
         status = 1;
         break;

      case IOCTL_TERM_CURSOR:
         draw_cursor = va_arg(ap, int) != 0;
         status = 0;
         break;

      default:
         break;
      }

      va_end(ap);

      return status;
   }

   virtual int write(const void* buffer, size_t n) override
   {
      const uint8_t* ptr = static_cast<const uint8_t*>(buffer);

      while(n--)
      {
         this->ansiWrite(*ptr++);
      }

      return n;
   }

   virtual int read(void* buffer_, size_t n) override
   {
      uint8_t* buffer = static_cast<uint8_t*>(buffer_);

      size_t i;

      for(i = 0; i < n; i++)
      {
         if (draw_cursor) this->drawCursor(Impl::Cursor::BLOCK);
         frame.refresh();

         uint8_t ch{};

         int status = getInput(ch);
         if(status <= 0)
         {
            this->drawCursor(Impl::Cursor::OFF);
            return status;
         }

         buffer[i] = ch;
         if (draw_cursor) this->drawCursor(Impl::Cursor::OFF);
      }

      return i;
   }

   using Device::read;
   using Device::write;
};

} // namespace TRM

#endif // TRM_FRAME_H
