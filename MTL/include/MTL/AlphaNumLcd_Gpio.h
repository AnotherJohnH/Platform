//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief HD44780 compatible alphanumeric LCD support via raw GPIO

#pragma once

#include "MTL/Gpio.h"

#include <unistd.h>

namespace MTL {

template <unsigned PIN_DATA,
          unsigned PIN_R_S,
          unsigned PIN_ENABLE,
          unsigned COLS,
          unsigned ROWS,
          bool     DL_8BIT = true>
class AlphaNumLcd
{
public:
   AlphaNumLcd()
   {
      usleep(40000);

      // Initialisation involves sending the "function" cmd four times
      function(/* dl_8bit */ true);
      usleep(4100);

      function(/* dl_8bit */ true);
      usleep(100);

      function(/* dl_8bit */ true);
      usleep(100);

      function(DL_8BIT);

      control(/* on */ false, /* cursor */ false, /* blink */ false);
      control(/* on */ true,  /* cursor */ false, /* blink */ false);

      entryMode(/* inc */ true, /* shift */ false);

      clear();
   }

   //! Clear display and home
   void clear()
   {
      col = row = 0;

      send(0b00000001);
      usleep(1520);
   }

   //! Move cursor to home
   void home()
   {
      col = row = 0;

      send(0b00000010);
      usleep(1520);
   }

   //! Set entry mode
   void entryMode(bool inc, bool shift)
   {
      uint8_t cmd = 0b00000100;

      if (inc)   cmd |= 0b10;
      if (shift) cmd |= 0b01;

      send(cmd);
   }

   //! Display and cusror control
   void control(bool on, bool cursor, bool blink)
   {
      uint8_t cmd = 0b00001000;

      if (on)     cmd |= 0b100;
      if (cursor) cmd |= 0b010;
      if (blink)  cmd |= 0b001;

      send(cmd);
   }

   //! Move cursor
   void move(unsigned col_, unsigned row_)
   {
      col = col_ < COLS ? col_ : 0;
      row = row_ < ROWS ? row_ : 0;

      send(0b10000000 | (row << 6) | col);
   }

   //! Send single char to the display
   void putchar(char ch)
   {
      if (ch != '\n')
      {
         send(ch, /* ram */ true);
      }

      if ((col++ == COLS) || (ch == '\n'))
      {
         col = 0;

         if (row++ == ROWS)
         {
            row = 0;
         }

         move(col, row);
      }
   }

   //! Write string to display
   void print(const char* s)
   {
      while(*s)
      {
         putchar(*s++);
      }
   }

   //! Program a character
   void progChar(uint8_t ch, const uint8_t* bitmap)
   {
      send(0b01000000 | ((ch & 0b111) << 3));

      for(unsigned r = 0; r < 8; ++r)
         send(bitmap[r]);

      move(col, row);
   }

private:
   //! Transfer 8 or 4 bits to LCD
   void tx(uint8_t data, bool ram)
   {
      port_data   = data;
      port_r_s    = ram;
      port_enable = true;
      usleep(10);

      port_enable = false;
      usleep(38);
   }

   //! Send command to LCD
   void send(uint8_t data, bool ram = false)
   {
      if (DL_8BIT)
      {
         tx(data, ram);
      }
      else
      {
         tx(data >> 4, ram);

         if (dl_4bit)
         {
            tx(data & 0xF, ram);
         }
      }
   }

   //! Initial function control
   void function(bool dl_8bit, bool font = false)
   {
      uint8_t cmd = 0b00100000;

      if (dl_8bit)   cmd |= 0b10000;
      if (ROWS == 2) cmd |= 0b01000;
      if (font)      cmd |= 0b00100;

      dl_4bit = not dl_8bit;

      send(cmd);
   }

   static const unsigned BITS = DL_8BIT ? 8 : 4;

   MTL::Gpio::Out<BITS, PIN_DATA>   port_data;
   MTL::Gpio::Out<1,    PIN_R_S>    port_r_s;
   MTL::Gpio::Out<1,    PIN_ENABLE> port_enable;

   uint8_t col;
   uint8_t row;
   bool    dl_4bit {false}; //!< 4-bit interface enabled
};

} // namespace MTL
