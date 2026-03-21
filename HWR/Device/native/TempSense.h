//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

class TempSens
{
public:
   TempSens() = default;

   void start() {}

   signed read()
   {
      if (up)
      {
         value += rate;
         if (value > max)
         {
            up = false;
         }
      }
      else
      {
         value -= rate;
         if (value < min)
         {
            up = true;
         }
      }

      return value;
   }

private:
   signed max{(21 << 8) + 0x40};
   signed min{( 9 << 8) + 0x80};
   signed rate{1};
   bool   up{false};
   signed value{15 << 8};
};
