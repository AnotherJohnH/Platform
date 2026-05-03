//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdio.h"
#include "stdint.h"
#include "ctype.h"

template <typename TYPE>
static char* write_int(char*    buffer,
                       size_t   n,
                       TYPE     value,
                       unsigned base,
                       unsigned width,
                       bool     left_justify,
                       bool     leading_zero,
                       bool     upper = false)
{
   char* s = buffer;

   if (value == 0)
   {
      *s++ = '0';
   }
   else
   {
      char a = upper ? 'A' : 'a';

      while(value != 0)
      {
         unsigned digit = value % base;
         if (digit > 9)
         {
            *s++ = a + digit - 10;
         }
         else
         {
            *s++ = '0' + digit;
         }
         value = value / base;
      }
   }

   // Pad
   unsigned m = s - buffer;
   if (m < width)
   {
      for(unsigned i=0; i<(width - m); i++)
      {
         *s++ = leading_zero ? '0' : ' ';
      }
   }

   // Reverse
   n = s - buffer;
   for(unsigned i=0; i<(n/2); i++)
   {
      char temp = buffer[i];
      buffer[i] = buffer[n - i - 1];
      buffer[n - i - 1] = temp;
   }

   return s;
}

static char* write_str(char*       buffer,
                       size_t      n,
                       const char* value)
{
   char* s = buffer;

   while((s - buffer) < (n - 1))
   {
      char ch = *value++;
      if (ch == '\0') break;
      *s++ = ch;
   }

   return s;
}

int vsnprintf(char* buffer, size_t n, const char* format, va_list ap)
{
   char* s = buffer;

   while(true)
   {
      int space = n - (s - buffer) - 1;
      if (space <= 0)
         break;

      char ch = *format++;

      if (ch == '\0')
      {
         break;
      }
      else if (ch == '%')
      {
         ch = *format++;

         bool left_justify = false;
         bool include_sign = false;
         bool leading_zero = false;

         while(true)
         {
                  if (ch == '-') left_justify = true;
             else if (ch == '+') include_sign = true;
             else if (ch == '0') leading_zero = true;
             else                break;

             ch = *format++;
         }

         unsigned width = 0;
         while(isdigit(ch))
         {
            width = width * 10 + ch - '0';
            ch = *format++;
         }

         switch(ch)
         {
         case 'b':
            {
               unsigned value = va_arg(ap, unsigned);
               s = write_int(s, space, value, 2, width, left_justify, leading_zero);
            }
            break;

         case 'o':
            {
               unsigned value = va_arg(ap, unsigned);
               s = write_int(s, space, value, 8, width, left_justify, leading_zero);
            }
            break;

         case 'd':
         case 'i':
            {
               int value = va_arg(ap, int);
               if (value < 0)
               {
                  value = -value;
                  *s++ = '-';
               }
               else if (include_sign)
               {
                  *s++ = '+';
               }

               s = write_int(s, space, value, 10, width, left_justify, leading_zero);
            }
            break;

         case 'u':
            {
               unsigned value = va_arg(ap, unsigned);
               s = write_int(s, space, value, 10, width, left_justify, leading_zero);
            }
            break;

         case 'x':
         case 'X':
            {
               unsigned value = va_arg(ap, unsigned);
               s = write_int(s, space, value, 16, width, left_justify, leading_zero, ch == 'X');
            }
            break;

         case 'c':
            {
               char value = char(va_arg(ap, int));
               *s++ = value;
            }
            break;

         case 's':
            {
               const char* value = va_arg(ap, const char*);
               s = write_str(s, space, value);
            }
            break;

         case 'p':
            {
               uintptr_t value = (uintptr_t)va_arg(ap, void*);
               s = write_int(s, space, value, 16, sizeof(uintptr_t) * 2,
                             /* left_justify */ false, /* leading_zero */ true);
            }
            break;

         default:
            *s++ = ch;
            break;
         }
      }
      else
      {
         *s++ = ch;
      }
   }

   *s = '\0';

   return s - buffer;
}
