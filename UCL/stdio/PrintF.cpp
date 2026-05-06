//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/PrintF.h"

#include "stdint.h"
#include "ctype.h"

template <typename TYPE>
static void write_int(PrintF*  buffer,
                      TYPE     value,
                      unsigned base,
                      int      width,
                      char     pad,
                      char     sign  = '\0',
                      bool     upper = false)
{
   char  local[32];
   char* s = local;

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

   bool left_justify{false};
   if (width < 0)
   {
      left_justify = true;
      width        = -width;
   }

   if (sign != '\0')
      --width;

   signed pad_len = width - (s - local);
   if (pad_len < 0)
      pad_len = 0;

   if (not left_justify)
   {
      for(unsigned i = 0; i < pad_len; i++)
      {
         buffer->putc(pad);
      }
   }

   if (sign != '\0')
      *s++ = sign;

   // Write out buffer in reverse order
   while(s > local)
   {
      buffer->putc(*--s);
   }

   if (left_justify)
   {
      for(unsigned i = 0; i < pad_len; i++)
      {
         buffer->putc(pad);
      }
   }
}

static void write_flt(PrintF* buffer,
                      float   value,
                      int     precision,
                      int     width,
                      char    pad,
                      char    sign)
{
   write_int(buffer, int(value), 10, width - precision - 1, pad, sign);
              
   buffer->putc('.');
                  
   if (precision > 0)
   {           
      value -= int(value);

#if !defined(PDK_ATTINY84) && !defined(PDK_ATTINY85)
      static unsigned pow10[] = { 10, 100, 1000, 10000, 100000 };
#else       
      static unsigned pow10[] = { 10, 100, 1000, 10000 };
#endif         
   
      unsigned frac = value * pow10[precision - 1] + 0.5;
               
      write_int(buffer, frac, 10, precision, '0');
   }
}    

static void write_str(PrintF* buffer, const char* value, int width)
{
   while(true)
   {
      char ch = *value++;
      if (ch == '\0') break;
      buffer->putc(ch);
   }
}

int PrintF::vprintf(const char* format, va_list ap)
{
   while(true)
   {
      char ch = *format++;

      if (ch == '\0')
      {
         break;
      }
      else if (ch == '%')
      {
         ch = *format++;

         bool left_justify = false;
         char sign         = '\0';
         char pad          = ' ';
         int  width        = 0;
         int  precision    = 0;

         while(true)
         {
                  if (ch == '-') left_justify = true;
             else if (ch == '+') sign         = '+';
             else if (ch == ' ') sign         = ' ';
             else if (ch == '0') pad          = '0';
             else                break;

             ch = *format++;
         }

         if (left_justify)
            pad = ' ';

         if (ch == '*')
         {
            width = va_arg(ap, int);
            ch    = *format++;
         }
         else if (isdigit(ch))
         {
            do
            {
               width = width * 10 + ch - '0';
               ch = *format++;
            }
            while(isdigit(ch));

            if (left_justify)
               width = -width;
         }

         if (ch == '.')
         {
            ch  = *format++;
            pad = ' ';

            if (ch == '*')
            {
               precision = va_arg(ap, int);
               ch = *format++;
            }
            else
            {
               while(isdigit(ch))
               {
                  precision = precision * 10 + ch - '0';
                  ch = *format++;
               } 
            } 
         }

         switch(ch)
         {
         case 'd':
         case 'i':
            {
               int value = va_arg(ap, int);
               if (value < 0)
               {
                  value = -value;
                  sign = '-';
               }
               write_int(this, value, 10, width, pad, sign);
            }
            break;

         case 'b':
            write_int(this, va_arg(ap, unsigned), 2, width, pad);
            break;

         case 'o':
            write_int(this, va_arg(ap, unsigned), 8, width, pad);
            break;

         case 'u':
            write_int(this, va_arg(ap, unsigned), 10, width, pad);
            break;

         case 'x':
         case 'X':
            write_int(this, va_arg(ap, unsigned), 16, width, pad, '\0', ch == 'X');
            break;

         case 'c':
            this->putc(char(va_arg(ap, int)));
            break;

         case 's':
            write_str(this, va_arg(ap, const char*), width);
            break;

         case 'p':
            write_str(this, "0x", /* width */ 0);
            write_int(this, uintptr_t(va_arg(ap, void*)), 16, sizeof(uintptr_t) * 2, '0');
            break;

         case 'f':
         case 'F':
         case 'e':
         case 'E':
         case 'g':
         case 'G':
            {
               float value = va_arg(ap, double);
               char sign{'\0'};
               if (value < 0)
               {
                  value = -value;
                  sign = '-';
               }
               write_flt(this, value, precision, width, pad, sign);
            }
            break;

         case 'n':
            {
               int* ptr = va_arg(ap, int*);
               *ptr = this->size();
            }
            break;

         default:
            this->putc(ch);
            break;
         }
      }
      else
      {
         this->putc(ch);
      }
   }

   return this->size();
}
