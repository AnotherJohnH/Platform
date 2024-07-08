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
//
//  Stereo: One 16-bit left+right sample pair per 32-bit word written to the FIFO
//  Mono:   Two 16-bit samples per 32-bit word written to the FIFO

#pragma once

#include "Pio.h"

namespace MTL {

class PioI2S : public PIO::Asm
{
public:
   PioI2S(bool mono_ = false, bool lsb_lrclk_msb_sclk_ = true)
   {
      unsigned lrclk = lsb_lrclk_msb_sclk_ ? 0b01 : 0b10;
      unsigned sclk  = lsb_lrclk_msb_sclk_ ? 0b10 : 0b01;

      if (mono_)
      {
         progMono(lrclk, sclk);
      }
      else
      {
         progStereo(lrclk, sclk);
      }
   }

   template <typename TYPE>
   signed download(TYPE&    pio,
                   unsigned sample_freq,
                   unsigned pin_sd,
                   unsigned pin_lrclk_sclk)
   {
      // Allocate a state machine
      signed sd = pio.allocSM();
      if (sd < 0)
         return sd;

      // Write code to PIO
      pio.SM_program(sd, *this);

      // Configure state machine
      pio.SM_clock(    sd, sample_freq * 32 * clk_mul);
      pio.SM_pinOUT(   sd, pin_sd);
      pio.SM_pinSIDE(  sd, pin_lrclk_sclk, 2);
      pio.SM_configOSR(sd, 32, MTL::SHIFT_LEFT,
                       auto_pull ? MTL::AUTO_PULL : MTL::MANUAL,
                       /* join_tx */ true);

      pio.SM_exec(     sd, SET(PIO::X, 12).side(0b00).op() );

      return sd;
   }

private:
   void progStereo(unsigned lrclk, unsigned sclk)
   {
      clk_mul   = 2;
      auto_pull = true;

      PIO::Lbl left_loop;
      PIO::Lbl right_loop;

      side_set(2);

      wrap_target();

      lbl(left_loop);
         OUT(PIO::PINS, 1)                 .side(0b00);
         JMP(PIO::X_NE_Z_DEC, left_loop)   .side(sclk);
         OUT(PIO::PINS, 1)                 .side(       lrclk);
         SET(PIO::X, 14)                   .side(sclk | lrclk);

      lbl(right_loop);
         OUT(PIO::PINS, 1)                 .side(lrclk);
         JMP(PIO::X_NE_Z_DEC, right_loop)  .side(sclk | lrclk);
         OUT(PIO::PINS, 1)                 .side(0b00);
         SET(PIO::X, 14)                   .side(sclk);

      wrap();
   }

   void progMono(unsigned lrclk, unsigned sclk)
   {
      clk_mul   = 4;
      auto_pull = false;

      PIO::Lbl left_loop_1;
      PIO::Lbl right_loop_1;
      PIO::Lbl left_loop_2;
      PIO::Lbl right_loop_2;

      side_set(2);

         POP();
         MOV(PIO::Y, PIO::OSR);

      wrap_target();

      lbl(left_loop_1);
         OUT(PIO::PINS, 1)                 .side(0b00)          .delay(1);
         JMP(PIO::X_NE_Z_DEC, left_loop_1) .side(sclk)          .delay(1);
         OUT(PIO::PINS, 1)                 .side(       lrclk)  .delay(1);
         SET(PIO::X, 14)                   .side(sclk | lrclk)  .delay(1);

      lbl(right_loop_1);
         OUT(PIO::PINS, 1)                 .side(lrclk)         .delay(1);
         JMP(PIO::X_NE_Z_DEC, right_loop_1).side(sclk | lrclk)  .delay(1);
         OUT(PIO::PINS, 1)                 .side(0b00);
         NOP()                             .side(0b00);
         SET(PIO::X, 14)                   .side(sclk);
         MOV(PIO::Y, PIO::OSR)             .side(sclk);

      lbl(left_loop_2);
         OUT(PIO::PINS, 1)                 .side(0b00)          .delay(1);
         JMP(PIO::X_NE_Z_DEC, left_loop_2) .side(sclk)          .delay(1);
         OUT(PIO::PINS, 1)                 .side(       lrclk)  .delay(1);
         SET(PIO::X, 14)                   .side(sclk | lrclk)  .delay(1);

      lbl(right_loop_2);
         OUT(PIO::PINS, 1)                 .side(lrclk)         .delay(1);
         JMP(PIO::X_NE_Z_DEC, right_loop_2).side(sclk | lrclk)  .delay(1);
         OUT(PIO::PINS, 1)                 .side(0b00);
         POP()                             .side(0b00);
         SET(PIO::X, 14)                   .side(sclk);
         MOV(PIO::Y, PIO::OSR)             .side(sclk);

      wrap();
   }

   unsigned clk_mul;
   bool     auto_pull;
};


} // namespace MTL
