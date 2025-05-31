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

#pragma once

#include "MTL/Pins.h"
#include "MTL/chip/rp2xxx/Audio.h"

#include "Pio.h"

namespace MTL {

class PioI2S : public PIO::Asm
{
public:
   PioI2S(Audio::Format format_ = Audio::STEREO_16, bool lsb_lrclk_msb_sclk_ = true)
   {
      S_0_1 = lsb_lrclk_msb_sclk_ ? 0b01 : 0b10;
      S_1_0 = S_0_1 ^ 0b11;

      switch(format_)
      {
      case Audio::MONO_16:         progMono16();       break;
      case Audio::STEREO_16:       progStereo16();     break;
      case Audio::STEREO_PAIRS_16: progStereoPair16(); break;
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
      pio.SM_clock(    sd, sample_freq * I2S_BITS_PER_SAMPLE * 2 * clk_mul * 2);
      pio.SM_pinOUT(   sd, pin_sd);
      pio.SM_pinSIDE(  sd, pin_lrclk_sclk);
      pio.SM_configOSR(sd, I2S_BITS_PER_SAMPLE * 2, MTL::SHIFT_LEFT,
                       auto_pull ? MTL::AUTO_PULL : MTL::MANUAL,
                       /* join_tx */ true);

      unsigned x_init = auto_pull ? X_INIT - 2 : X_INIT;
      pio.SM_exec(sd, SET(PIO::X, x_init).side(S_0_0).op() );

      return sd;
   }

private:
   void progStereo16()
   {
      clk_mul   = 1;
      auto_pull = true;

      PIO::Lbl left_loop;
      PIO::Lbl right_loop;

      side_set(2);

      wrap_target();

      //###############################################################
      // OSR = R:L
      lbl(left_loop);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_0_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop)  .side(S_1_0);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_0_1);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)              .side(S_1_1);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:R
      lbl(right_loop);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_0_1);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop) .side(S_1_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                .side(S_0_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)              .side(S_1_0);
      //---------------------------------------------------------------

      wrap();
   }

   void progMono16()
   {
      clk_mul   = 2;
      auto_pull = false;

      PIO::Lbl left_loop_1;
      PIO::Lbl right_loop_1;
      PIO::Lbl left_loop_2;
      PIO::Lbl right_loop_2;

      side_set(2);

         POP();
         MOV(PIO::Y, PIO::OSR);

      wrap_target();

      //###############################################################
      // OSR = S2:S1 Y = S2:S1
      lbl(left_loop_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_0)  .delay(1);
      // NOP                                .side(S_0_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop_1)  .side(S_1_0)  .delay(1);
      // NOP                                .side(S_1_0);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_1)  .delay(1);
      // NOP                                .side(S_0_1);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)                .side(S_1_1);
         MOV(PIO::OSR, PIO::Y)              .side(S_1_1);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = S2:S1 Y = S2:S1
      lbl(right_loop_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_1)  .delay(1);
      // NOP                                .side(S_0_1);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop_1) .side(S_1_1)  .delay(1);
      // NOP                                .side(S_1_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_0)  .delay(1);
      // NOP                                .side(S_0_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)                .side(S_1_0);
         MOV(PIO::Y, PIO::OSR)              .side(S_1_0);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:S2 Y = 0:S2
      lbl(left_loop_2);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_0)  .delay(1);
      // NOP                                .side(S_0_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop_2)  .side(S_1_0)  .delay(1);
      // NOP                                .side(S_1_0);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_1)  .delay(1);
      // NOP                                .side(S_0_1);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)                .side(S_1_1);
         MOV(PIO::OSR, PIO::Y)              .side(S_1_1);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:S2 Y = 0:S2
      lbl(right_loop_2);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_1)  .delay(1);
      // NOP                                .side(S_0_1);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop_2) .side(S_1_1)  .delay(1);
      // NOP                                .side(S_1_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                  .side(S_0_0);
         POP()                              .side(S_0_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)                    .side(S_1_0);
         MOV(PIO::Y, PIO::OSR)              .side(S_1_0);
      //---------------------------------------------------------------

      wrap();
   }

   void progStereoPair16()
   {
      clk_mul   = 4;
      auto_pull = false;

      PIO::Lbl left_loop1;
      PIO::Lbl right_loop1;
      PIO::Lbl left_loop2;
      PIO::Lbl right_loop2;

      side_set(2);

         POP();

      wrap_target();

      //###############################################################
      // OSR = L2:L1

      lbl(left_loop1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_0)  .delay(3);
      // NOP                               .side(S_0_0);
      // NOP                               .side(S_0_0);
      // NOP                               .side(S_0_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop1)  .side(S_1_0)  .delay(3);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_1);
         MOV(PIO::Y, PIO::OSR)             .side(S_0_1);
         POP()                             .side(S_0_1)  .delay(1);
      // NOP                               .side(S_0_1);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)               .side(S_1_1)  .delay(3);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = R2:R1, Y = 0:L2

      lbl(right_loop1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_1)  .delay(3);
      // NOP                               .side(S_0_1);
      // NOP                               .side(S_0_1);
      // NOP                               .side(S_0_1);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop1) .side(S_1_1)  .delay(3);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_0);
         MOV(PIO::X, PIO::OSR)             .side(S_0_0);
         MOV(PIO::OSR, PIO::Y)             .side(S_0_0);
         MOV(PIO::Y, PIO::X)               .side(S_0_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)               .side(S_1_0)  .delay(3);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:L2 Y = 0:R2

      lbl(left_loop2);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_0)  .delay(3);
      // NOP                               .side(S_0_0);
      // NOP                               .side(S_0_0);
      // NOP                               .side(S_0_0);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, left_loop2)  .side(S_1_0)  .delay(3);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_1);
         MOV(PIO::OSR, PIO::Y)             .side(S_0_1)  .delay(2);
      // NOP                               .side(S_0_1);
      // NOP                               .side(S_0_1);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)               .side(S_1_1)  .delay(3);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      //---------------------------------------------------------------

      //###############################################################
      // OSR = 0:R2

      lbl(right_loop2);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_1)  .delay(3);
      // NOP                               .side(S_0_1);
      // NOP                               .side(S_0_1);
      // NOP                               .side(S_0_1);
      //---------------------------------------------------------------
         JMP(PIO::X_NE_Z_DEC, right_loop2) .side(S_1_1)  .delay(3);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      // NOP                               .side(S_1_1);
      //---------------------------------------------------------------
         OUT(PIO::PINS, 1)                 .side(S_0_0);
         POP()                             .side(S_0_0)  .delay(2);
      // NOP                               .side(S_0_0);
      // NOP                               .side(S_0_0);
      //---------------------------------------------------------------
         SET(PIO::X, X_INIT)               .side(S_1_0)  .delay(3);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      // NOP                               .side(S_1_0);
      //---------------------------------------------------------------

      wrap();
   }

   static const unsigned I2S_BITS_PER_SAMPLE = 16;
   static const unsigned X_INIT              = I2S_BITS_PER_SAMPLE - 2;

   uint8_t  S_0_0{0b00};
   uint8_t  S_0_1{0b01};
   uint8_t  S_1_0{0b10};
   uint8_t  S_1_1{0b11};
   unsigned clk_mul;
   bool     auto_pull;
};


} // namespace MTL
