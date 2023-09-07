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

// \brief Driver for shift registered 7-seg LED displays

#pragma once

#include "MTL/MTL.h"
#include "MTL/Digital.h"

namespace MTL {

template <unsigned PIN_CLK, unsigned PIN_DATA, unsigned NUM_DIGITS, bool COMMON_ANODE = false, unsigned DELAY = 10>
class Led7Seg
{
public:
   Led7Seg()
   {
      // Dirty state so that following clear is always sent
      // and so guarantees synchronization of state[] with
      // the external shift registers
      state[0] = 1;

      clear();
   }

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
   //! A simple uncalibrated delay
   void delay()
   {
      // The specifc frequency used to clock the shift registers is
      // unimportant. What is important is that the clock is not too
      // fast for the shift registers and not too slow for the application.
      // Measurements have found that a DELAY of 10 will result in a shift
      // reg clock of ~1/200th the CPU clock e.g...
      // => a CPU clock of 100 MHz will give a shift reg clock of 500 KHz
      // and an update time of 16 uS per digit
      volatile uint32_t n = DELAY;
      while(n--);
   }

   //! Transmit raw segment data (8 bits) for one 7-seg LED module
   void txRaw(uint8_t segments)
   {
      for(unsigned i = 0; i < 8; ++i)
      {
         port_clk  = false;
         port_data = segments & 1;
         segments >>= 1;
         delay();

         port_clk = true;
         delay();
      }
   }

   //! Update the state of all the LEDs
   void updateState(const uint8_t* new_state)
   {
      bool update {false};

      for(unsigned i = 0; i < NUM_DIGITS; ++i)
      {
         if (state[i] != new_state[i])
         {
            update = true;
            break;
         }
      }

      if (update)
      {
         // Only upodate external shift regitsres if state
         // has changed
         for(unsigned i = 0; i < NUM_DIGITS; ++i)
         {
            if (COMMON_ANODE)
               txRaw(~new_state[i]);
            else
               txRaw(new_state[i]);

            state[i] = new_state[i];
         }
      }
   }

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

   MTL::Digital::Out<PIN_CLK>  port_clk;
   MTL::Digital::Out<PIN_DATA> port_data;
   uint8_t                     state[NUM_DIGITS];
};

} // namespace MTL
