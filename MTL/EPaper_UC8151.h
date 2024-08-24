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
// \brief E-paper with UC8151 (or IL0373) display driver IC

#pragma once

#include <unistd.h>

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/CanvasMono1.h"

namespace MTL {

template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned PIN_DC,
          unsigned PIN_CS,
          unsigned PIN_RST,
          unsigned PIN_BUSY,
          typename SPI>
class EPaper_UC8151
{
public:
   using Canvas = CanvasMono1<EPaper_UC8151, /* SWAP_XY */ true, /* INVERT */ true>;

   EPaper_UC8151()
   {
      reset();

      init();
   }

   //! Display width (pixels)
   static constexpr unsigned getWidth() { return WIDTH; }

   //! Display height (pixels)
   static constexpr unsigned getHeight() { return HEIGHT; }

   //! Frame buffer width stride (bytes)
   static constexpr unsigned getStride() { return (WIDTH + 7) / 8; }

   //! Update the display
   void display(const uint8_t* buffer, bool partial = false)
   {
      sendCmd(CMD_PON);
      sendCmd(CMD_PTOUT);

      unsigned bytes_per_row = getStride();

      sendCmd(CMD_DTM2, HEIGHT * bytes_per_row, buffer);

      sendCmd(CMD_DSP);
      sendCmd(CMD_DRF);
      wait();
      sendCmd(CMD_POF);
   }

private:
   //! Apply a 20ms RST pulse
   void reset()
   {
      pin_rst_n = 0;
      usleep(10000);

      pin_rst_n = 1;
      usleep(10000);

      wait();
   }

   //! Wait for busy pin to clear
   void wait()
   {
      while(not pin_busy_n)
      {
         usleep(100);
      }
   }

   //! Tx a command byte
   void sendCmd(uint8_t cmd)
   {
      pin_cs_n = 0;
      pin_dc   = 0;
      usleep(T_CSSUP);

      spi.tx(cmd);

      usleep(T_CSHLD);
      pin_cs_n = 1;
   }

   //! Tx a data byte
   void sendData(uint8_t byte)
   {
      pin_cs_n = 0;
      pin_dc   = 1;
      usleep(T_CSSUP);

      spi.tx(byte);

      usleep(T_CSHLD);
      pin_cs_n = 1;
   }

   void sendCmd(uint8_t cmd, uint8_t a)
   {
      sendCmd(cmd); sendData(a);
   }

   void sendCmd(uint8_t cmd, uint8_t a, uint8_t b)
   {
      sendCmd(cmd); sendData(a); sendData(b);
   }

   void sendCmd(uint8_t cmd, uint8_t a, uint8_t b, uint8_t c)
   {
      sendCmd(cmd); sendData(a); sendData(b); sendData(c);
   }

   void sendCmd(uint8_t cmd, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
   {
      sendCmd(cmd); sendData(a); sendData(b); sendData(c); sendData(d);
   }

   void sendCmd(uint8_t cmd, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e)
   {
      sendCmd(cmd); sendData(a); sendData(b); sendData(c); sendData(d); sendData(e);
   }

   void sendCmd(uint8_t cmd, unsigned n, const uint8_t* buffer)
   {
      sendCmd(cmd);

      for(unsigned i = 0; i < n; ++i)
         sendData(buffer[i]);
   }

   void init()
   {
      sendCmd(CMD_PSR,
              /* RES_128x296 */ (2<<6) |
              /* LUT_OTP     */ (0<<5) |
              /* FORMAT_BW   */ (1<<4) |
              /* SCAN_DOWN   */ (1<<3) |
              /* SHIFT_LEFT  */ (1<<2) |
              /* BOOSTER_ON  */ (1<<1) |
              /* RESET_NONE  */ (1<<0));

      sendCmd(CMD_PWR,
              /* VDS_INTERNAL */ (1<<1) | /* VDG_INTERNAL */ (1<<0),
              /* VCOM_VD */      (0<<2) | /* VGHL_16V */     (0<<0),
              0b101011,
              0b101011,
              0b101011);

      sendCmd(CMD_PON);
      wait();

      sendCmd(CMD_BTST,
              /* START_10MS */ (0<<6) | /* STRENGTH_3 */ (2<<3) | /* OFF_6_58US */ 0b111,
              /* START_10MS */ (0<<6) | /* STRENGTH_3 */ (2<<3) | /* OFF_6_58US */ 0b111,
              /* START_10MS */ (0<<6) | /* STRENGTH_3 */ (2<<3) | /* OFF_6_58US */ 0b111);

      sendCmd(CMD_PFS,  /* FRAMES_1 */ (0<<4));

      sendCmd(CMD_TSE,  /* TEMP_INTERNAL */ (0<<7) | /* OFFSET_0 */ (0<<0));
      sendCmd(CMD_TCON, 0x22);
      sendCmd(CMD_CDI,  0b01001100); // inverted
      sendCmd(CMD_PLL,  /* HZ_100 */ 0b00111010);

      sendCmd(CMD_POF);
      wait();
   }

   // chip select (CS) timing
   static const unsigned T_CSSUP = 2; // (us) Setup time before SCLK rise (Doc says 60 nS)
   static const unsigned T_CSHLD = 2; // (us) Hold time after SCLK fall (Doc says 65 nS)

   // SPI commands
   const uint8_t CMD_PSR   = 0x00;
   const uint8_t CMD_PWR   = 0x01;
   const uint8_t CMD_POF   = 0x02;
   const uint8_t CMD_PFS   = 0x03;
   const uint8_t CMD_PON   = 0x04;
   const uint8_t CMD_PMES  = 0x05;
   const uint8_t CMD_BTST  = 0x06;
   const uint8_t CMD_DSLP  = 0x07;
   const uint8_t CMD_DTM1  = 0x10;
   const uint8_t CMD_DSP   = 0x11;
   const uint8_t CMD_DRF   = 0x12;
   const uint8_t CMD_DTM2  = 0x13;
   const uint8_t CMD_LUTC  = 0x20;
   const uint8_t CMD_LUTWW = 0x21;
   const uint8_t CMD_LUTBW = 0x22;
   const uint8_t CMD_LUTWB = 0x23;
   const uint8_t CMD_LUTBB = 0x24;
   const uint8_t CMD_PLL   = 0x30;
   const uint8_t CMD_TSC   = 0x40;
   const uint8_t CMD_TSE   = 0x41;
   const uint8_t CMD_TSW   = 0x42;
   const uint8_t CMD_TSR   = 0x43;
   const uint8_t CMD_CDI   = 0x50;
   const uint8_t CMD_LPD   = 0x51;
   const uint8_t CMD_TCON  = 0x60;
   const uint8_t CMD_TRES  = 0x61;
   const uint8_t CMD_REV   = 0x70;
   const uint8_t CMD_FLG   = 0x71;
   const uint8_t CMD_AMV   = 0x80;
   const uint8_t CMD_VV    = 0x81;
   const uint8_t CMD_VDCS  = 0x82;
   const uint8_t CMD_PTL   = 0x90;
   const uint8_t CMD_PTIN  = 0x91;
   const uint8_t CMD_PTOUT = 0x92;
   const uint8_t CMD_PWS   = 0xE3;

   static const unsigned SPI_CLOCK = 24000000; // 24 MHz

   MTL::Digital::Out<PIN_DC>  pin_dc;
   MTL::Digital::Out<PIN_CS>  pin_cs_n;
   MTL::Digital::Out<PIN_RST> pin_rst_n;
   MTL::Digital::In<PIN_BUSY> pin_busy_n;
   SPI                        spi{SPI_CLOCK, /* use_cs */ false, /* use_miso */ false};
};

}
