//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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
//------------------------------------------------------------------------------

// \brief HD44780 compatible alphanumeric LCD support via an
//        PCF8574 I2C bridge

#pragma once

#include "MTL/MTL.h"

#include <unistd.h>

namespace MTL {

template <typename I2C, unsigned COLS, unsigned ROWS>
class AlphaNumLcd
{
public:
   AlphaNumLcd(uint8_t i2c_addr_ = 0x27)
   {
      i2c.setTarget(i2c_addr_);
      i2c.write(0, /* stop */ true);

      // Wait 20ms to power up
      usleep(20000);

      // Send the reset "function" cmd three times
      function(/* dl_8bit */ true, /* two_lines */ false);
      usleep(4100);

      function(/* dl_8bit */ true, /* two_lines */ false);
      usleep(100);

      function(/* dl_8bit */ true, /* two_lines */ false);
      usleep(100);

      // => 4-bit mode
      function(/* dl_8bit */ false, /* two_lines */ false);
      usleep(100);

      back_light = true;

      function(/* dl_8bit */ false, /* two_lines */ ROWS == 2);

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
   //! Transfer 4 bits to the LCD
   void tx(uint8_t data4, bool ram)
   {
      static const uint8_t BIT_RS = 0b0001;
      static const uint8_t BIT_RW = 0b0010;
      static const uint8_t BIT_E  = 0b0100;
      static const uint8_t BIT_BT = 0b1000;

      uint8_t byte = (data4 << 4);

      if (ram)
         byte |= BIT_RS;

      if (back_light)
         byte |= BIT_BT;

      i2c.write(byte | BIT_E, /* stop */ true);

      usleep(10);

      i2c.write(byte, /* stop */ true);

      usleep(38);
   }

   //! Send command to LCD
   void send(uint8_t data, bool ram = false)
   {
      tx(data >> 4,  ram);
      tx(data & 0xF, ram);
   }

   //! Initial function control
   void function(bool dl_8bit, bool two_lines)
   {
      uint8_t cmd = 0b00100000;

      if (dl_8bit)   cmd |= 0b10000;
      if (two_lines) cmd |= 0b01000;

      send(cmd);
   }

   I2C     i2c{};
   bool    back_light{false};
   uint8_t col{0};
   uint8_t row{0};
};

} // namespace MTL
