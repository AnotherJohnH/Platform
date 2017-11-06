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

//! \file strtol.c
//! \brief tiny C library implementation

#include <stdlib.h>

long strtol(const char* s, const char** endptr, int base)
{
   long  value = 0;

   if (base == 0)
   {
      if (*s == '0')
      {
         s++;

         if ((*s == 'x') || (*s == 'X'))
         {
            s++;
            base = 16;
         }
         else
         {
            base = 8;
         }
      }
      else
      {
         base = 10;
      }
   }

   while(1)
   {
      char     ch = *s++;
      unsigned digit;

      if ((ch >= '0') && (ch <='9'))
      {
         digit = ch - '0';
      }
      else if ((ch >= 'a') && (ch <='z'))
      {
         digit = ch - 'a' + 10;
      }
      else if ((ch >= 'A') && (ch <='Z'))
      {
         digit = ch - 'A' + 10;
      }
      else
      {
         break;
      }

      if (digit >= base) break;

      value = value * base + digit;
   }

   if (endptr)
   {
      *endptr = s;
   }

   return value;
}
