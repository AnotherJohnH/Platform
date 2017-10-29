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

#ifndef TRM_CONSOLE_H
#define TRM_CONSOLE_H

#include <cstdarg>
#include <cstdlib>
#include <cstdio>

#include <termios.h>
#include <sys/select.h>

#include "PLT/KeyCode.h"

#include "STB/Fifo.h"

#include "TRM/Device.h"

namespace TRM {

//! Terminal device using the console as the back-end
class Console : public Device
{
private:
   STB::Fifo<int, 2> input;
   unsigned timeout_ms{0};

   static termios* getTio()
   {
      static termios tio;
      return &tio;
   }

   static void restoreTio() { tcsetattr(0, TCSANOW, getTio()); }

   static void saveTio()
   {
      tcgetattr(0, getTio());

      atexit(Console::restoreTio);
   }

   static void modifyTioFlag(unsigned flag, bool set)
   {
      struct termios tio;

      tcgetattr(0, &tio);

      if(set)
         tio.c_lflag |= flag;
      else
         tio.c_lflag &= ~flag;

      tcsetattr(0, TCSANOW, &tio);
   }

   int readStdin()
   {
      if(timeout_ms != 0)
      {
         fd_set fds;
         FD_ZERO(&fds);
         FD_SET(0, &fds); // fd 0 is assumed to be stdin

         struct timeval timeout;
         timeout.tv_sec  = timeout_ms / 1000;
         timeout.tv_usec = (timeout_ms % 1000) * 1000;

         int status = select(1, &fds, nullptr, nullptr, &timeout);
         if(status <= 0) return status;
      }

      return ::fgetc(stdin);
   }

   int inputPush()
   {
      int ch = readStdin();
      input.push(ch);
      return ch;
   }

   int getNextChar()
   {
      if(input.empty())
      {
         if(inputPush() == 0x1B)
         {
            if(inputPush() == 0x5B)
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

      int ch = input.back();
      input.pop();
      return ch;
   }

public:
   Console(const char* title_) { saveTio(); }

   ~Console() { restoreTio(); }

   // Implement TRM::Device

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

      case IOCTL_TERM_TIMEOUT_MS:
         timeout_ms = va_arg(ap, unsigned);
         status     = 0;
         break;

      case IOCTL_TERM_COLOURS:
         // Assume ANSI 256 colours are available by the host terminal
         status = 256;
         break;

      case IOCTL_TERM_FONTS:
         status = 1;
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

      size_t i;

      for(i = 0; i < n; i++)
      {
         ::putchar(*ptr++);
      }

      return i;
   }

   virtual int read(void* buffer, size_t n) override
   {
      uint8_t* ptr = static_cast<uint8_t*>(buffer);

      size_t i;

      for(i = 0; i < n; i++)
      {
         int ch = getNextChar();
         if(ch <= 0) return ch;

         *ptr++ = ch;
      }

      return i;
   }
};

} // namesapce TRM

#endif // TRM_CONSOLE_H
