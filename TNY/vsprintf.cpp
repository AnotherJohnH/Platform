//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

static char* itos(char*    buffer,
                  uint32_t value,
                  unsigned base,
                  unsigned width,
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
   unsigned n = s - buffer;
   if (n < width)
   {
      for(unsigned i=0; i<(width - n); i++)
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


int vsprintf(char* buffer, const char* format, va_list ap)
{
   char* s = buffer;

   while(true)
   {
      char ch = *format++;
      if (ch == '\0')
      {
         break;
      }
      else if (ch == '%')
      {
         bool     leading_zero = false;
         unsigned width = 0;

         ch = *format++;
         if (ch == '0')
         {
            leading_zero = true;
            ch = *format++;
         }

         while(isdigit(ch))
         {
            width = width * 10 + ch - '0';
            ch = *format++;
         }

         switch(ch)
         {
         case 'o':
            {
               unsigned value = va_arg(ap, unsigned);
               s = itos(s, value, 8, width, leading_zero);
            }
            break;

         case 'd':
            {
               int value = va_arg(ap, int);
               if (value < 0)
               {
                  value = -value;
                  *s++ = '-';
               }
               s = itos(s, value, 10, width, leading_zero);
            }
            break;

         case 'u':
            {
               unsigned value = va_arg(ap, unsigned);
               s = itos(s, value, 10, width, leading_zero);
            }
            break;

         case 'x':
         case 'X':
            {
               unsigned value = va_arg(ap, unsigned);
               s = itos(s, value, 16, width, leading_zero, ch == 'X');
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
               while(true)
               {
                  ch = *value++;
                  if (ch == '\0') break;
                  *s++ = ch;
               }
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
