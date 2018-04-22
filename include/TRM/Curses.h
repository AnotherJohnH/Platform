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

#ifndef TRM_CURSES_H
#define TRM_CURSES_H

#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "Device.h"

namespace TRM {

static const unsigned A_UNDERLINE = 1 << 0;
static const unsigned A_REVERSE   = 1 << 1;
static const unsigned A_NORMAL    = 1 << 2;
static const unsigned A_BOLD      = 1 << 3;
static const unsigned A_DIM       = 1 << 4;
static const unsigned A_ITALIC    = 1 << 5;
static const unsigned A_FIXED     = 1 << 6;

//! Very small curses 'like' wrapper for TRM devices
class Curses
{
public:
   unsigned lines{};
   unsigned cols{};

   Curses(Device* dev_ = nullptr) { setDevice(dev_); }

   ~Curses() { setDevice(nullptr); }

   void setDevice(Device* dev_)
   {
      if(dev != nullptr) dev->close();

      dev = dev_;

      if(dev != nullptr)
      {
         dev->open(0);

         init();
      }
   }

   void init()
   {
      move(999, 999);
      getyx(lines, cols);
      move(1, 1);
   }

   //! Set raw mode
   void raw() { dev->ioctl(Device::IOCTL_TERM_ICANON, 0); }

   //! Enable echo
   void echo() { dev->ioctl(Device::IOCTL_TERM_ECHO, 1); }

   //! Disable echo
   void noecho() { dev->ioctl(Device::IOCTL_TERM_ECHO, 0); }

   //! Move cursor to the specified position
   void move(int r, int c)
   {
      addstr("\033[");
      adduint(unsigned(r));
      addch(';');
      adduint(unsigned(c));
      addch('H');
   }

   //! Add a character and advance the cursor
   void addch(const char ch) { dev->write(&ch, 1); }

   //! Add a null terminated string of characters and advance the cursor
   void addstr(const char* s)
   {
      while(*s)
      {
         addch(*s++);
      }
   }

   //! Add a string of n characters and advance the cursor
   void addnstr(const char* s, int n)
   {
      for(int i = 0; s[i] && (i < n); i++)
      {
         addch(s[i]);
      }
   }

   //! Add a character at the specified position and advance the cursor
   void mvaddch(int r, int c, const char ch)
   {
      move(r, c);
      addch(ch);
   }

   //! Add a null terminated string of characters at the specified position and advance the cursor
   void mvaddstr(int r, int c, const char* s)
   {
      move(r, c);
      addstr(s);
   }

   //! Add a string of n characters at the specified position and advance the cursor
   void mvaddnstr(int r, int c, const char* s, int n)
   {
      move(r, c);
      addnstr(s, n);
   }

   //! Clear the screen
   void clear() { addstr("\033[2J\033[H"); }

   //! Clear to the end of the line
   void clrtoeol() { addstr("\033[2K"); }

   //! Turn on all the attributes in the mask
   void attron(unsigned attr_) { attrset(attr | attr_); }

   //! Turn off all the attributes in the mask
   void attroff(unsigned attr_) { attrset(attr & ~attr_); }

   //! Set all the attributes in the mask
   void attrset(unsigned attr_)
   {
      attr = attr_;
      addstr("\033[0");
      if(attr & A_BOLD)      addstr(";1");
      if(attr & A_ITALIC)    addstr(";3");
      if(attr & A_UNDERLINE) addstr(";4");
      if(attr & A_REVERSE)   addstr(";7");
      if(attr & A_FIXED)     addstr(";11");
      addstr("m");
   }

   //! Set the foreground colour (0-7 or 9)
   void fgcolour(unsigned col)
   {
      if((col == 8) || (col > 9)) return;

      addstr("\033[");
      adduint(30 + col);
      addch('m');
   }

   //! Set the extended foreground colour (0-255)
   void extfgcolour(unsigned col)
   {
      addstr("\033[38;5;");
      adduint(col & 0xFF);
      addch('m');
   }

   //! Set the background colour (0-7 or 9)
   void bgcolour(unsigned col)
   {
      if((col == 8) || (col > 9)) return;

      addstr("\033[");
      adduint(40 + col);
      addch('m');
   }

   //! Set the extended foreground colour (0-255)
   void extbgcolour(unsigned col)
   {
      addstr("\033[48;5;");
      adduint(col & 0xFF);
      addch('m');
   }

   //! Select a font
   void fontset(unsigned font)
   {
      if(font > 10) return;

      addstr("\033[");
      adduint(10 + font);
      addch('m');
   }

   //! Get the current cursor position
   void getyx(unsigned& y, unsigned& x)
   {
      y = x = 0;

      // TODO fix this it would break apps requiring echo
      noecho();
      raw();

      addstr("\033[6n");

      char response[16];

      for(unsigned i = 0; true; i++)
      {
         if((i == sizeof(response)) || (dev->read(response + i, 1) == -1)) return;

         if(response[i] == 'R') break;
      }

      const char* s = response;

      if(*s++ != '\033') return;
      if(*s++ != '[') return;

      const char* t = strchr(s, ';');
      if(t == nullptr) return;

      y = atoi(s);
      x = atoi(t + 1);
   }

   //! set input timeout (mS)
   int timeout(int timeout_ms_)
   {
      int prev_timeout_ms = timeout_ms;
      timeout_ms          = timeout_ms_;
      return prev_timeout_ms;
   }

   //! Get next character input
   int getch() const
   {
      uint8_t ch;
      int     status;

      if(timeout_ms != 0)
      {
         dev->ioctl(Device::IOCTL_TERM_TIMEOUT_MS, timeout_ms);
         status = dev->read(&ch, 1);
         dev->ioctl(Device::IOCTL_TERM_TIMEOUT_MS, 0);
      }
      else
      {
         status = dev->read(&ch, 1);
      }

      return status > 0 ? ch : status;
   }

private:
   Device* dev{nullptr};
   unsigned attr{0};
   unsigned timeout_ms{0};

   void adduint(unsigned value)
   {
      unsigned v = value;
      for(unsigned place = 1000000000; place != 0; place = place / 10)
      {
         unsigned digit = v / place;
         if((digit > 0) || (value > place))
         {
            addch('0' + digit);
         }
         v = v - digit * place;
      }
   }
};

} // namespace TRM

#endif // TRM_CURSES_H
