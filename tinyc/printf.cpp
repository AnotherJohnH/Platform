//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

//! \file printf.cpp
//! \brief tiny C library implementation


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>


static void print_u(unsigned value, unsigned base, int width=10, char pad = ' ')
{
   uint8_t  digit[32];
   int      num_digits;

   for(num_digits=0; (value > 0) && (num_digits < 32); num_digits++)
   {
      uint8_t d = value % base;
      value = value / base;
      digit[num_digits] = d > 9 ? d - 10 + 'A' : d + '0';
   }

   if (num_digits == 0)
   {
      digit[0] = '0';
      num_digits = 1;
   }

   for(int i=0; i<(width - num_digits); i++)
   {
      putchar(pad);
   }

   for(int i=0; i<num_digits; i++)
   {
      putchar(digit[num_digits - 1 - i]);
   }
}

static void print_i(int value, unsigned base, int width=10, char pad = ' ')
{
   if (value < 0)
   {
      putchar('-');
      value = -value;
      width--;
   }

   print_u(value, base, width, pad);
}

#ifdef FLOAT
static void print_f(double value, int width, unsigned places)
{
   print_i((int)value, 10, width - places - 1, ' ');

   putchar('.');

   if (places > 0)
   {
      value -= (int) value;

      static unsigned pow10[] = { 10, 100, 1000, 10000, 100000 };

      unsigned frac = value * pow10[places - 1] + 0.5;

      print_u(frac, 10, places, '0');
   }
}
#endif

void vprintf(const char* format, va_list ap)
{
   for(const char* s = format; *s; s++)
   {
      char ch = *s;

      if (ch == '%')
      {
         ch = *++s;

         if (ch == '%') break;

         char pad = ' ';

         if (ch == '0')
         {
            pad = '0';
            ch = *++s;
         }

         unsigned field_width = 0;
         unsigned places = 0;

         while( (ch >= '0') && (ch <= '9'))
         {
            field_width = field_width*10 + ch - '0';
            ch = *++s;
         }

         if (ch == '.')
         {
            ch = *++s;

            while( (ch >= '0') && (ch <= '9'))
            {
               places = places*10 + ch - '0';
               ch = *++s;
            }
         }

         switch(ch)
         {
         case 'b':
            print_u(va_arg(ap, unsigned),  2, field_width, pad);
            break;

         case 'o':
            print_u(va_arg(ap, unsigned),  8, field_width, pad);
            break;

         case 'u':
            print_u(va_arg(ap, unsigned), 10, field_width, pad);
            break;

         case 'x':
         case 'X':
            print_u(va_arg(ap, unsigned), 16, field_width, pad);
            break;

         case 'd':
            print_i(va_arg(ap, unsigned), 10, field_width, pad);
            break;

#ifdef FLOAT
         case 'f':
            print_f(va_arg(ap, double), field_width, places);
            break;
#endif

         case 'p':
            print_u((unsigned)va_arg(ap, void*), 16, 8, '0');
            break;

         case 's':
            puts(va_arg(ap, const char*));
            break;

         case 'c':
            putchar(va_arg(ap, int));
            break;

         default:
            break;
         }
      }
      else
      {
         putchar(ch);
      }
   }
}


int printf(const char* format, ...)
{
   va_list  ap;

   va_start(ap, format);
   vprintf(format, ap);
   va_end(ap);

   return 0;
}
