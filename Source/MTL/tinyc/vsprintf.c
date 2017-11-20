/*------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------*/

//! \file vsprintf.c
//! \brief tiny C library implementation

#include <stdio.h>

int vsprintf(char* buffer, const char* format, va_list ap)
{
   char* s = buffer;

   while(*format)
   {
      if (*format == '%')
      {
         format++;

         switch(*format)
         {
         case 'd':
            {
               int value = va_arg(ap, int);
               (void) value;
               // TODO
            }
            break;

         // TODO more cases here!

         default:
            *s = *format;
            break;
         }

         format++;
      }
      else
      {
         *s = *format;
      }

      format++;
   }

   *s = '\0';

   return s - buffer;
}
