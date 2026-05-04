//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace UCL {

template <typename TYPE>
TYPE strtou(const char* s, const char** endptr, unsigned base)
{
   TYPE value = 0;

   if (base == 0)
   {
      if (*s == '0')
      {
         s++;

              if ((*s == 'x') || (*s == 'X')) { s++; base = 16; }
         else if ((*s == 'b') || (*s == 'B')) { s++; base = 2; }
         else                                 { base = 8; }
      }
      else
      {
         base = 10;
      }
   }

   while(true)
   {
      char     ch = *s++;
      unsigned digit;

           if ((ch >= '0') && (ch <='9')) digit = ch - '0';
      else if ((ch >= 'a') && (ch <='z')) digit = ch - 'a' + 10;
      else if ((ch >= 'A') && (ch <='Z')) digit = ch - 'A' + 10;
      else                                break;

      if (digit >= base) break;
      if ((value * base) < value) break;

      value = value * base + digit;
   }

   if (endptr != nullptr)
      *endptr = s;

   return value;
}

template <typename TYPE>
TYPE strto(const char* s, const char** endptr, unsigned base)
{
   bool neg{false};

        if (*s == '-') { neg = true; s++; }
   else if (*s == '+') { s++; }

   TYPE value = strtou<TYPE>(s, endptr, base);

   return neg ? -value
              : +value;
}

} // namespace UCL
