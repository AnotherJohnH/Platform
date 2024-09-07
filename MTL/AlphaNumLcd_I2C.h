//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

// \brief I2C AIP31068L or compatible alphanumeric LCD

#pragma once

#include <unistd.h>

#include "MTL/MTL.h"

namespace MTL {

template <typename I2C, unsigned COLS, unsigned ROWS>
class AlphaNumLcd
{
public:
   AlphaNumLcd(uint8_t i2c_addr_ = 0x3E)
   {
      i2c.setTarget(i2c_addr_);

      usleep(15000);

      function();
      display(/* on */ true,  /* cursor */ false, /* blink */ false);
      clear();
      entryMode(/* inc */ true, /* shift */ false);
   }

   //! Clear display and home
   void clear()
   {
      col = row = 0;

      sendCmd(0b00000001);
      usleep(1530);
   }

   //! Move cursor to home
   void home()
   {
      col = row = 0;

      sendCmd(0b00000010);
      usleep(1530);
   }

   //! Set entry mode
   void entryMode(bool inc, bool shift)
   {
      uint8_t cmd = 0b00000100;

      if (inc)   cmd |= 0b10;
      if (shift) cmd |= 0b01;

      sendCmd(cmd);
   }

   //! Display and cusror control
   void display(bool on, bool cursor, bool blink)
   {
      uint8_t cmd = 0b00001000;

      if (on)     cmd |= 0b100;
      if (cursor) cmd |= 0b010;
      if (blink)  cmd |= 0b001;

      sendCmd(cmd);
   }

   //! Move cursor
   void move(unsigned col_, unsigned row_)
   {
      uint8_t cmd = 0b10000000;

      col = col_ < COLS ? col_ : 0;
      row = row_ < ROWS ? row_ : 0;

      sendCmd(cmd | (row << 6) | col);
   }

   //! Send single char to the display
   void putchar(char ch)
   {
      if (ch != '\n')
      {
         sendRam(ch, true);
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
      sendCmd(0b01000000 | ((ch & 0b111) << 3));

      for(unsigned r = 0; r < 8; ++r)
         sendRam(bitmap[r], r == 7);

      move(col, row);
   }

private:
   //! Send command to LCD
   void sendRam(uint8_t data, bool stop)
   {
       i2c.write(0x40, false); // No further command (b7) and RAM (b6)
       i2c.write(data, stop);
       usleep(100);
   }

   //! Send command to LCD
   void sendCmd(uint8_t data)
   {
       i2c.write(0x00, false); // No further command (b7) and not RAM (b6)
       i2c.write(data, true);
       usleep(100);
   }

   //! Initial function control
   void function(bool font = false)
   {
      uint8_t cmd = 0b00100000;

      cmd |= 0b10000;
      if (ROWS == 2) cmd |= 0b01000;
      if (font)      cmd |= 0b00100;

      sendCmd(cmd);
   }

   I2C     i2c{};
   uint8_t col{0};
   uint8_t row{0};
};

} // namespace MTL
