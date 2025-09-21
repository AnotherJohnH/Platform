//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file strto.h
//! \brief tiny C library implementation

#ifndef TINYC_STRTO_H
#define TINYC_STRTO_H

namespace TNY {

template <typename TYPE>
TYPE strto(const char* s, const char** endptr, unsigned base)
{
   TYPE value = 0;
   int sign = +1;

   if (*s == '-')
   {
      sign = -1;
      s++;
   }
   else if (*s == '+')
   {
      s++;
   }

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

      // TODO check for overflow
      value = value * base + digit;
   }

   if (endptr)
   {
      *endptr = s;
   }

   return value * sign;
}

} // namespace TNY

#endif
