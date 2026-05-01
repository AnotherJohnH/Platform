//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

#include "ctype.h"

double strtod(const char* s, const char** endptr)
{
   long     whole = strtol(s, &s, 10);
   unsigned frac{0};
   unsigned places{0};
   long     exponent{0};

   if (*s == '.')
   {
      s++;
      const char* end;
      frac   = strtol(s, &end, 10);
      places = end - s;
   }

   if ((*s == 'e') || (*s == 'E'))
   {
      exponent = strtol(s + 1, &s, 10);
   }

   if (endptr != nullptr)
      *endptr = s;

   double value = whole;

   if (places != 0)
   {
      // TODO
      // double denominator = pow(10.0, places);
      // value += frac / denominator;
   }

   if (exponent != 0)
   {
      // TODO
      //value *= pow(10.0, exponent);
   }

   return value;
}
