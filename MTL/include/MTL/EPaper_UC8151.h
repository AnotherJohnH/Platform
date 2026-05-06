//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief E-paper with UC8151 (or IL0373) display driver IC
//
// Data source was the Pimoroni-pico repo

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
      init();
   }

   //! Display width (pixels)
   static constexpr unsigned getWidth() { return WIDTH; }

   //! Display height (pixels)
   static constexpr unsigned getHeight() { return HEIGHT; }

   //! Frame buffer width stride (bytes)
   static constexpr unsigned getStride() { return (HEIGHT + 7) / 8; }

   //! Update the display
   void display(const uint8_t* buffer, bool quick = false)
   {
      if (quick != fast)
      {
         // fast = quick; XXX not good enough quality
         init();
      }

      sendCmd(CMD_PON);
      sendCmd(CMD_PTOUT);

      sendCmd(CMD_DTM2, WIDTH * getStride(), buffer);

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
      reset();

      sendCmd(CMD_PSR,
              /* RES_128x296 */ (2<<6) |
              /* LUT_OTP     */ ((fast ? 1 : 0)<<5) |
              /* FORMAT_BW   */ (1<<4) |
              /* SCAN_DOWN   */ (1<<3) |
              /* SHIFT_LEFT  */ (1<<2) |
              /* BOOSTER_ON  */ (1<<1) |
              /* RESET_NONE  */ (1<<0));

      if (fast)
         setFastLUTs();

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

   void setDefaultLUTs()
   {
   }

   void setMediumLUTs()
   {
      const uint8_t vcom[] =
      {
         0x00, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x00, 0x23, 0x23, 0x00, 0x00, 0x02,
         0x00, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00
      };

      sendCmd(CMD_LUTC, sizeof(vcom), vcom);

      const uint8_t ww[] =
      {
         0x54, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x60, 0x23, 0x23, 0x00, 0x00, 0x02,
         0xa8, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTWW, sizeof(ww), ww);

      const uint8_t bw[] =
      {
         0x54, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x60, 0x23, 0x23, 0x00, 0x00, 0x02,
         0xa8, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTBW, sizeof(bw), bw);

      const uint8_t wb[] =
      {
         0xa8, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x60, 0x23, 0x23, 0x00, 0x00, 0x02,
         0x54, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTWB, sizeof(wb), wb);

      const uint8_t bb[] =
      {
         0xa8, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x60, 0x23, 0x23, 0x00, 0x00, 0x02,
         0x54, 0x16, 0x16, 0x0d, 0x00, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTBB, sizeof(bb), bb);

      wait();
   }

   void setFastLUTs()
   {
      const uint8_t vcom[] =
      {
         0x00, 0x04, 0x04, 0x07, 0x00, 0x01,
         0x00, 0x0c, 0x0c, 0x00, 0x00, 0x02,
         0x00, 0x04, 0x04, 0x07, 0x00, 0x02,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00
      };

      sendCmd(CMD_LUTC, sizeof(vcom), vcom);

      const uint8_t ww[] =
      {
         0x54, 0x04, 0x04, 0x07, 0x00, 0x01,
         0x60, 0x0c, 0x0c, 0x00, 0x00, 0x02,
         0xa8, 0x04, 0x04, 0x07, 0x00, 0x02,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTWW, sizeof(ww), ww);

      const uint8_t bw[] =
      {
         0x54, 0x04, 0x04, 0x07, 0x00, 0x01,
         0x60, 0x0c, 0x0c, 0x00, 0x00, 0x02,
         0xa8, 0x04, 0x04, 0x07, 0x00, 0x02,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTBW, sizeof(bw), bw);

      const uint8_t wb[] =
      {
         0xa8, 0x04, 0x04, 0x07, 0x00, 0x01,
         0x60, 0x0c, 0x0c, 0x00, 0x00, 0x02,
         0x54, 0x04, 0x04, 0x07, 0x00, 0x02,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTWB, sizeof(wb), wb);

      const uint8_t bb[] =
      {
         0xa8, 0x04, 0x04, 0x07, 0x00, 0x01,
         0x60, 0x0c, 0x0c, 0x00, 0x00, 0x02,
         0x54, 0x04, 0x04, 0x07, 0x00, 0x02,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };

      sendCmd(CMD_LUTBB, sizeof(bb), bb);

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
   bool                       fast{false};
};

}
