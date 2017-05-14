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

#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include "ZConsole.h"


static FILE*  input_fp = nullptr;
static FILE*  print_fp = nullptr;
static FILE*  snoop_fp = nullptr;


ZConsole::ZConsole(PLT::Device* device_)
   : Curses(device_)
{
   raw();
   noecho();

   input_fp = fopen("test.in", "r");
   if (input_fp)
   {
       // Fixed width
       cols = 80;
   }
   else
   {
      input_fp = fopen("fast.in", "r");
   }

   print_fp = NULL;
   snoop_fp = NULL;
}

ZConsole::~ZConsole()
{
   if (print_fp) fclose(print_fp);
   if (snoop_fp) fclose(snoop_fp);
}


void ZConsole::print(uint16_t zscii)
{
   // Filter repeated new-line
   static unsigned newline_count = 1;
   if (zscii == 0xD)
   {
      zscii = '\n';
   }
   if (zscii == '\n')
   {
      if (++newline_count >= 3) return;
   }
   else
   {
      newline_count = 0;
   }

   if (print_fp == NULL) print_fp = fopen("print.log", "w");
   assert(print_fp);
   fputc(zscii, print_fp);
}

void ZConsole::snoop(uint16_t zscii)
{
   if (snoop_fp == NULL) snoop_fp = fopen("snoop.in", "w");
   assert(snoop_fp);
   fputc(zscii, snoop_fp);
}

bool ZConsole::read(uint16_t& zscii, unsigned timeout)
{
   // TODO timeout
   if ((input_fp == NULL) || feof(input_fp))
   {
      input_fp = NULL;
      int ch = getch();
      if (ch < 0)
      {
         exit(0);
      }
      if (ch == 0x7F) ch = '\b';
      zscii = ch;
   }
   else
   {
      zscii = fgetc(input_fp);
   }
   return true;
}

void tprintf(const char* format, ...)
{
   static FILE* fp = NULL;
   va_list  ap;

   if (fp == NULL)
   {
      fp = fopen("zif.log", "w");
   }

   va_start(ap, format);
   vfprintf(fp, format, ap);
   va_end(ap);

   fflush(fp);
}

