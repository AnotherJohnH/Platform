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

#ifndef PLT_CURSES_H
#define PLT_CURSES_H

#include <cstdlib>
#include <cstring>
#include <cstdint>

#include "PLT/Device.h"

namespace PLT {

enum : unsigned
{
   A_UNDERLINE = 1<<0,
   A_REVERSE   = 1<<1,
   A_NORMAL    = 1<<2,
   A_BOLD      = 1<<3,
   A_DIM       = 1<<4,
   A_ITALIC    = 1<<5,
   A_FIXED     = 1<<6
};

//! Very small curses like wrapper for PLT devices
class Curses
{
public:
   unsigned  lines{};
   unsigned  cols{};

   Curses(Device* dev_ = nullptr)
   {
      setDevice(dev_);
   }

   ~Curses()
   {
      setDevice(nullptr);
   }

   void setDevice(Device* dev_)
   {
      if (dev != nullptr) dev->close();

      dev = dev_;

      if (dev != nullptr)
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
   void raw()
   {
      dev->ioctl(Device::IOCTL_TERM_ICANON, 0);
   }

   //! Enable echo
   void echo()
   {
      dev->ioctl(Device::IOCTL_TERM_ECHO, 1);
   }

   //! Disable echo
   void noecho()
   {
      dev->ioctl(Device::IOCTL_TERM_ECHO, 0);
   }

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
   void addch(const char ch)
   {
      dev->write(&ch, 1);
   }

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
      for(int i=0; s[i] && (i<n); i++)
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
   void clear()
   {
      addstr("\033[2J\033[H");
   }

   //! Clear to the end of the line
   void clrtoeol()
   {
      addstr("\033[2K");
   }

   //! Turn on all the attributes in the mask
   void attron(unsigned attr_)
   {
      attrset(attr | attr_);
   }

   //! Turn off all the attributes in the mask
   void attroff(unsigned attr_)
   {
      attrset(attr & ~attr_);
   }

   //! Set all the attributes in the mask
   void attrset(unsigned attr_)
   {
      attr = attr_;
      addstr("\033[0");
      if (attr & A_BOLD)       addstr(";1");
      if (attr & A_ITALIC)     addstr(";3");
      if (attr & A_UNDERLINE)  addstr(";4");
      if (attr & A_REVERSE)    addstr(";7");
      if (attr & A_FIXED)      addstr(";11");
      addstr("m");
   }

   //! Set the foreground and background colours
   void colourset(unsigned fg_col, unsigned bg_col)
   {
      addstr("\033[");
      adduint(30+fg_col-2);
      addch(';');
      adduint(40+bg_col-2);
      addch('m');
   }

   //! Get the current cursor position
   void getyx(unsigned& y, unsigned& x)
   {
      y = x = 0;

      addstr("\033[6n");

      char response[16];

      for(unsigned i=0; true; i++)
      {
         if ((i == sizeof(response)) ||
             (dev->read(response + i, 1) == -1)) return;

         if (response[i] == 'R') break;
      }

      const char* s = response;

      if (*s++ != '\033') return;
      if (*s++ != '[')    return;

      const char* t  = strchr(s, ';');
      if (s == 0) return;

      y = atoi(s);
      x = atoi(t + 1);
   }

   //! Get next character input
   int getch()
   {
      uint8_t ch;
      return dev->read(&ch, 1) == -1 ? -1 : ch;
   }

private:
   Device*   dev{nullptr};
   unsigned  attr{0};

   void adduint(unsigned value)
   {
      unsigned v = value;
      for(unsigned place=1000000000; place != 0; place = place / 10)
      {
         unsigned digit = v / place;
         if ((digit > 0) || (value > place))
         {
            addch('0' + digit);
         }
         v = v - digit * place;
      }
   }
};

} // namespace PLT

#endif
