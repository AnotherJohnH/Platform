//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdarg>
#include <cstdlib>
#include <cstdio>

#include <termios.h>
#include <sys/select.h>

#include "PLT/KeyCode.h"

#include "STB/Fifo.h"

#include "TRM/Device.h"

namespace TRM {

//! Terminal device implementation using stdio
class ConsoleDevice : public Device
{
public:
   ConsoleDevice(const char* title_) { saveTio(); }

   ~ConsoleDevice() { restoreTio(); }

protected:
   // Implement TRM::Device

   int ioctl(unsigned request, ...) override
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

      case IOCTL_TERM_CURSOR:
         if (va_arg(ap, int) != 0)
         {
            ::puts("\e[?25h");
         }
         else
         {
            ::puts("\e[?25l");
         }
         status = 0;
         break;

      default:
         break;
      }

      va_end(ap);

      return status;
   }

   int write(const void* buffer, size_t n) override
   {
      const uint8_t* ptr = static_cast<const uint8_t*>(buffer);

      size_t i;

      for(i = 0; i < n; i++)
      {
         ::putchar(*ptr++);
      }

      return i;
   }

   int read(void* buffer, size_t n) override
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

private:
   static termios* getTio()
   {
      static termios tio;
      return &tio;
   }

   static void restoreTio()
   {
      tcsetattr(0, TCSANOW, getTio());
      ::puts("\e[?25h");
   }

   static void saveTio()
   {
      tcgetattr(0, getTio());

      atexit(ConsoleDevice::restoreTio);
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
            switch(inputPush())
            {
            case 'O':
               switch(inputPush())
               {
               case 'P': input.clear(); return PLT::F1;
               case 'Q': input.clear(); return PLT::F2;
               case 'R': input.clear(); return PLT::F3;
               case 'S': input.clear(); return PLT::F4;
               }
               break;

            case '[':
               switch(inputPush())
               {
               case 'A': input.clear(); return PLT::UP;
               case 'B': input.clear(); return PLT::DOWN;
               case 'C': input.clear(); return PLT::RIGHT;
               case 'D': input.clear(); return PLT::LEFT;
               }
               break;
            }
         }
      }

      int ch = input.back();
      input.pop();
      return ch;
   }

   STB::Fifo<int, 2> input;
   unsigned          timeout_ms{0};
};

} // namesapce TRM
