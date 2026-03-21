//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Simulated LED interface

#pragma once

#include "Panel.h"

template <unsigned NUM_DIGITS, unsigned SCALE = 2>
class Led7Seg
{
public:
   Led7Seg() = default;

   //! Turn off all LEDs
   void clear()
   {
      uint8_t buffer[NUM_DIGITS];

      for(unsigned i = 0; i < NUM_DIGITS; ++i)
      {
         buffer[i] = 0;
      }

      updateState(buffer);
   }

   //! Send value to daisy chained 7-seg modules
   void print(unsigned value, unsigned base = 10, unsigned dp = NO_DP)
   {
      uint8_t buffer[NUM_DIGITS];

      for(unsigned i = 0; i < NUM_DIGITS; ++i)
      {
         buffer[i] = convertDigit(value % base, dp == i);
         value /= base;
      }

      updateState(buffer);
   }

   //! Send hex (or BCD) value to daisy chained 7-seg modules
   void printHex(uint32_t value, unsigned dp = NO_DP)
   {
      print(value, /* base */ 16, dp);
   }

   //! Send decimal value to daisy chained 7-seg modules
   void printDec(unsigned value, unsigned dp = NO_DP)
   {
      print(value, /* base */ 10, dp);
   }

   static const unsigned NO_DP = NUM_DIGITS;

private:
   //! Convert a single hex digit to a segments bit mask
   static uint8_t convertDigit(unsigned digit, bool dp = false)
   {
      static const uint8_t table_edc_bafg[16] =
      {
   //      edc.bafg
         0b11101110, // 0
         0b00101000, // 1
         0b11001101, // 2
         0b01101101, // 3

         0b00101011, // 4
         0b01100111, // 5
         0b11100111, // 6
         0b00101100, // 7

         0b11101111, // 8
         0b01101111, // 9
         0b10101111, // a
         0b11100011, // b

         0b11000110, // c
         0b11101001, // d
         0b11000111, // e
         0b10000111  // f
      };

      uint8_t segments = table_edc_bafg[digit];

      if (dp)
         segments |= 0b00010000;

      return segments;
   }

   void updateState(const uint8_t* new_state)
   {
      bool update{false};

      for(unsigned i = 0; i < NUM_DIGITS; ++i)
      {
         if (state[i] != new_state[i])
         {
            state[i ] = new_state[i];
            update = true;
         }
      }

      if (update)
         draw();

      panel.eventPoll();
   }

   void draw()
   {
      panel.clear(COL_BGND);

      for(unsigned i = 0; i < NUM_DIGITS; ++i)
      {
         unsigned x    = 39 * (NUM_DIGITS - i - 1);
         uint8_t  segs = state[i];

         panel.drawLine(0x000000, x + DIGIT_WIDTH - 1, 0, x + DIGIT_WIDTH - 1, HEIGHT);

         panel.fillRect(segs & 0x04 ? COL_ON : COL_OFF, x +  8,  4, x + 26,  8); // a
         panel.fillRect(segs & 0x08 ? COL_ON : COL_OFF, x + 26,  9, x + 30, 21); // b
         panel.fillRect(segs & 0x20 ? COL_ON : COL_OFF, x + 26, 27, x + 30, 39); // c
         panel.fillRect(segs & 0x40 ? COL_ON : COL_OFF, x +  8, 40, x + 26, 44); // d
         panel.fillRect(segs & 0x80 ? COL_ON : COL_OFF, x +  4, 27, x +  8, 39); // e
         panel.fillRect(segs & 0x02 ? COL_ON : COL_OFF, x +  4,  9, x +  8, 21); // f
         panel.fillRect(segs & 0x01 ? COL_ON : COL_OFF, x +  8, 22, x + 26, 26); // g

         panel.fillCircle(segs & 0x10 ? COL_ON : COL_OFF, x + 34, 42, 2);        // .
      }

      panel.refresh();
   }

   static const STB::Colour COL_BGND = STB::RGB(0x20, 0x20, 0x20);
   static const STB::Colour COL_OFF  = STB::RGB(0x40, 0x40, 0x40);
   static const STB::Colour COL_ON   = STB::RGB(0xFF, 0x00, 0x00);

   static const unsigned DIGIT_WIDTH = 39;
   static const unsigned WIDTH       = DIGIT_WIDTH * NUM_DIGITS;
   static const unsigned HEIGHT      = 51;

   Panel<WIDTH,HEIGHT,SCALE> panel{};

   uint8_t state[NUM_DIGITS];
};
