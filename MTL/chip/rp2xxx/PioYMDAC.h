//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

#pragma once

#include "MTL/chip/Pio.h"

namespace MTL {

//! Input peripheral for YM3012 DAC serial data
template <typename PIO_TYPE>
class PioYMDAC : public PIO::Asm
{
public:
   PioYMDAC()
   {
      PIO::Lbl chan_loop;
      PIO::Lbl skip_loop;
      PIO::Lbl data_loop;
      PIO::Lbl exp_loop;

         // Ensure SAM1 is high before starting
         WFC(PIO::PIN_HI, IN_SAM1);

      // Read a complete DAC frame (32-bits)
      // skipping the first 3-bits and inverting the last 4 bits
      wrap_target();

         // Wait for falling edge of SAM1 to sync to 1st channel
         WFC(PIO::PIN_LO, IN_SAM1);

         // For each channel
         SET(PIO::Y, NUM_CHAN - 1);
         lbl(chan_loop);

            // Skip initial bits (3 bits)
            SET(PIO::X, 3 - 1);
            lbl(skip_loop);
               WFC(PIO::PIN_LO, IN_CLK);
               WFC(PIO::PIN_HI, IN_CLK);
            JMP(PIO::X_NE_Z_DEC, skip_loop);

            // Push in LS two bits as zero
            INP(PIO::ZERO, 2);

            // Push in next bit as one
            SET(PIO::X, 1);
            INP(PIO::X, 1);

         // Read LS bits of mantissa (9 bits)
            SET(PIO::X, 9 - 1);
            lbl(data_loop);
               WFC(PIO::PIN_LO, IN_CLK);
               WFC(PIO::PIN_HI, IN_CLK);
               INP(PIO::PINS, 1);
            JMP(PIO::X_NE_Z_DEC, data_loop);

         // Read and invert MS bit of mantissa and exp (4-bits)
            INV(PIO::ISR, PIO::ISR);
            SET(PIO::X, 4 - 1);
            lbl(exp_loop);
               WFC(PIO::PIN_LO, IN_CLK);
               WFC(PIO::PIN_HI, IN_CLK);
               INP(PIO::PINS, 1);
            JMP(PIO::X_NE_Z_DEC, exp_loop);
            INV(PIO::ISR, PIO::ISR);

         JMP(PIO::Y_NE_Z_DEC, chan_loop);

         // Push partially processed sample pair into FIFO
         PSH(PIO::NO_BLOCK);

      wrap();
   }

   //! Download program to PIO and configure a state machine
   signed download(unsigned clock_freq_,
                   unsigned pin_dac_sd_sam1_clk_)
   {
      // Allocate a state machine
      sd = pio.allocSM();
      if (sd < 0)
         return -1;

      // Write code to PIO
      if (pio.SM_program(sd, *this) < 0)
         return -1;

      // Configure state machine
      pio.SM_clock(    sd, clock_freq_ * 2 * 4);
      pio.SM_pinINP(   sd, pin_dac_sd_sam1_clk_, 3);
      pio.SM_configISR(sd, DAC_FRAME_BITS, MTL::SHIFT_RIGHT,
                       MTL::MANUAL, /* join_rx */ true);

      return sd;
   }

   //! Start the peripheral
   void start()
   {
      pio.start(1 << sd);
   }

   //! Read packed 16-bit 2's complement sample pair
   void pop(int16_t& left_, int16_t& right_)
   {
      uint32_t data = pio.SM_pop(sd);

      // Raw data is already partially processed by the PIO
      // into packed "floating-point" sample pair with the
      // following format...
      //
      //   332 2222222211111 111 11
      //   109 8765432109876 543 2109876543210
      //  +---+-------------+---+-------------+
      //  |AAA|BBBBBBBBBB100|CCC|DDDDDDDDDD100|
      //  +---+-------------+---+-------------+
      //
      //  A,C - 3-bit exponents (0-6) in effect right shift to apply to mantissa
      //  B,D - 10-bit 2's complement mantissa extended with 0b100

      // Convert packed 16-bit floating point pair
      // to 16-bit 2's complement pair
      left_  = decodeFloat16(data & 0xFFFF);
      right_ = decodeFloat16(data >> 16);
   }

private:
   //! Convert 16-bit "floating-point" value to 16-bit 2's compliment
   static signed decodeFloat16(uint16_t float16_)
   {
      unsigned exp      = float16_ >> 13;
      int16_t  mantissa = float16_ << 3;

      return mantissa >> exp;
   }

   static const unsigned NUM_CHAN       = 2;
   static const unsigned CHANNEL_BITS   = 16;
   static const unsigned DAC_FRAME_BITS = NUM_CHAN * CHANNEL_BITS;

   static const unsigned IN_SD   = 0;
   static const unsigned IN_SAM1 = 1;
   static const unsigned IN_CLK  = 2;

   PIO_TYPE pio{};
   signed   sd{-1};
};

} // namespace MTL
