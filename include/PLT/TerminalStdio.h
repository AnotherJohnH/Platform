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

#ifndef PLT_TERMINAL_STDIO_H
#define PLT_TERMINAL_STDIO_H

#include <termios.h>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

#include "PLT/Device.h"
#include "PLT/KeyCode.h"

#include "STB/Fifo.h"

namespace PLT {

//! Terminal device using stdio as the back-end
class TerminalStdio : public PLT::Device
{
private:
   STB::Fifo<uint8_t,2>  input;

   static termios* getTio()
   {
      static termios tio;
      return &tio;
   }

   static void restoreTio()
   {
      tcsetattr(0, TCSANOW, getTio());
   }

   static void saveTio()
   {
      tcgetattr(0, getTio());

      atexit(TerminalStdio::restoreTio);
   }

   static void modifyTioFlag(unsigned flag, bool set)
   {
      struct termios tio;

      tcgetattr(0, &tio);

      if (set)
         tio.c_lflag |= flag;
      else
         tio.c_lflag &= ~flag;

      tcsetattr(0, TCSANOW, &tio);
   }

   uint8_t inputPush()
   {
      uint8_t ch = ::fgetc(stdin);
      input.push(ch);
      return ch;
   }

   uint8_t getNextChar()
   {
      if (input.empty())
      {
         if (inputPush() == 0x1B)
         {
            if (inputPush() == 0x5B)
            {
               switch(inputPush())
               {
               case 'A': input.clear(); return PLT::UP;
               case 'B': input.clear(); return PLT::DOWN;
               case 'C': input.clear(); return PLT::RIGHT;
               case 'D': input.clear(); return PLT::LEFT;
               }
            }
         }
      }

      uint8_t ch = input.back();
      input.pop();
      return ch;
   }

public:
   TerminalStdio(const char* title_)
   {
      saveTio();
   }

   ~TerminalStdio()
   {
      restoreTio();
   }

   // Implement PLT::Device

   virtual int ioctl(unsigned request, ...) override
   {
      int     status = -1;
      va_list ap;

      va_start(ap, request);

      switch(request)
      {
      case IOCTL_TERM_ICANON:
         modifyTioFlag(ICANON, va_arg(ap, int) != 0);
         status = 0;
         break;

      case IOCTL_TERM_ECHO:
         modifyTioFlag(ECHO, va_arg(ap, int) != 0);
         status = 0;
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

      size_t i;

      for(i = 0; i < n; i++)
      {
         ::putchar(*ptr++);
      }

      return i;
   }

   virtual ssize_t read(void* buffer, size_t n) override
   {
      uint8_t* ptr = static_cast<uint8_t*>(buffer);

      size_t i;

      for(i = 0; i < n; i++)
      {
         *ptr++ = getNextChar();
      }

      return i;
   }
};

} // namespace PLT

#endif
