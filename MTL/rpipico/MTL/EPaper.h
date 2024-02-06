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
// \brief Waveshare 2.13" E-paper display (V3)

#pragma once

#include <unistd.h>

#include "MTL/MTL.h"
#include "MTL/Digital.h"

#include "MTL/rp2040/Spi.h"

namespace MTL {

class EPaper_213_V3
{
public:
   EPaper_213_V3()
   {
      pin_cs  = 1;
      pin_rst = 1;

      usleep(20000);
   }

   //! Display width (pixels)
   static constexpr unsigned getWidth() { return WIDTH; }

   //! Display height (pixels)
   static constexpr unsigned getHeight() { return HEIGHT; }

   //! Frame buffer width stride (bytes)
   static constexpr unsigned getStride() { return (WIDTH + 7) / 8; }

   //! Clear display to white
   void clear(bool partial = false)
   {
      if (partial)
         partialWakeup();
      else
         fullWakeup();

      sendCmd(CMD_WRITE_RAM_BW);

      unsigned bytes_per_row = getStride();

      for(unsigned i = 0; i < (bytes_per_row * HEIGHT); ++i)
      {
         sendData(0xFF);
      }

      turnOn(partial);

      sleep();
   }

   //! Update the display with partial update
   void display(const uint8_t* buffer, bool partial = false)
   {
      if (partial)
         partialWakeup();
      else
         fullWakeup();

      sendCmd(CMD_WRITE_RAM_BW, getStride() * HEIGHT, buffer);

      turnOn(partial);

      sleep();
   }

   static const unsigned WIDTH  = 122;
   static const unsigned HEIGHT = 250;

private:
   //! Apply a 2ms RST pulse
   void reset()
   {
      pin_rst = 0;
      usleep(2000);

      pin_rst = 1;
      usleep(120000);
   }

   //! Wait for busy pin to clear
   void wait()
   {
      while(pin_busy)
      {
         usleep(10000);
      }
      usleep(10000); // XXX eh! copied from vendors example
   }

   //! Tx a command byte
   void sendCmd(uint8_t cmd)
   {
      pin_dc = 0;
      pin_cs = 0;
      usleep(T_CSSUP);

      spi.tx(cmd);

      usleep(T_CSHLD);
      pin_cs = 1;
   }

   //! Tx a data byte
   void sendData(uint8_t byte)
   {
      pin_dc = 1;
      pin_cs = 0;
      usleep(T_CSSUP);

      spi.tx(byte);

      usleep(T_CSHLD);
      pin_cs = 1;
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

   void sendCmd(uint8_t cmd, unsigned n, const uint8_t* buffer)
   {
      sendCmd(cmd);

      for(unsigned i = 0; i < n; ++i)
         sendData(buffer[i]);
   }

   void turnOn(bool partial = false)
   {
      sendCmd(CMD_DISP_UPDATE_CTRL2, partial ? 0x0F : 0xC7);
      sendCmd(CMD_MASTER_ACTIVATION);
      wait();
   }

   void setWindow(unsigned x1, unsigned y1, unsigned x2, unsigned y2)
   {
      sendCmd(CMD_RAM_X_ADDR_START_END, x1 / 8, x2 / 8);
      sendCmd(CMD_RAM_Y_ADDR_START_END, y1, y1 >> 8, y2, y2 >> 8);
   }

   void setCursor(unsigned x, unsigned y)
   {
      sendCmd(CMD_RAM_X_ADDR_CNTR, x);
      sendCmd(CMD_RAM_Y_ADDR_CNTR, y, y >> 8);
   }

   void loadLUT(const uint8_t* lut)
   {
      sendCmd(CMD_LUT, 153, lut);
      wait();

      sendCmd(0x3F, lut[153]); // XXX cargo culted across from the WaveShare example code

      sendCmd(CMD_GATE_DRV_V, /* VGH */ lut[154]);
      sendCmd(CMD_SRC_DRV_V,  /* VSH1 */ lut[155], /* VSH2 */ lut[156], /* VSL */ lut[157]);
      sendCmd(CMD_VCOM,       lut[158]);
   }

   void loadFullLUT()
   {
      static const uint8_t lut[159] =
      {                    
         0x80, 0x4A, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x40, 0x4A, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x80, 0x4A, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x40, 0x4A, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x0F, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x02,
         0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                    
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                    
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00,
         0x22, 0x17, 0x41, 0x00, 0x32, 0x36
      };

      loadLUT(lut);
   }

   void loadPartialLUT()
   {
      static const uint8_t lut[159] =
      {   
          0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00,
          0x22, 0x17, 0x41, 0x00, 0x32, 0x36
      };

      loadLUT(lut);
   }

   //! Reset the display ready for a new image
   void fullWakeup()
   {
      reset();
      wait();

      sendCmd(CMD_SWRST);
      wait();

      sendCmd(CMD_DRV_OUT_CTRL,    /* A[7:0] */ 0xF9, /* A[8] */ 0x00, /* B[2:1] */ 0b000);
      sendCmd(CMD_DATA_ENTRY_MODE, /* AM = X, Y = inc, X = inc */ 0b011);

      setWindow(0, 0, WIDTH - 1, HEIGHT - 1);
      setCursor(0, 0);

      sendCmd(CMD_BRDR_WAVFRM,
          /* VBD option */ (0b00 << 6) |
          /* VBD level */  (0b00 << 4) |
          /* GS control */ ( 0b1 << 2) |
          /* GS setting */ (0b01 << 0) );

      sendCmd(CMD_DISP_UPDATE_CTRL1,
          /* Red RAM option = normal */      (0b000 << 4) |
          /* Blk RAM option = normal */      (0b000 << 0),
          /* Source output mode = S0-S175 */ (0b1   << 7));
 
      sendCmd(CMD_TEMP_SENSE_CTRL, /* = internal */ 0x80);
      wait();

      loadFullLUT();
   }

   //! Reset the display ready for a new image with partial update
   void partialWakeup()
   {
      reset();
      wait();

      loadPartialLUT();

      const uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00};
      sendCmd(0x37, sizeof(data), data);

      sendCmd(CMD_BRDR_WAVFRM,
          /* VBD option */ (0b10 << 6) |
          /* VBD level */  (0b00 << 4) |
          /* GS control */ ( 0b0 << 2) |
          /* GS setting */ (0b00 << 0) );

      // Display Update Sequence Option
      // Enable clock and  Enable analog
      sendCmd(CMD_DISP_UPDATE_CTRL2, 0xC0);

      sendCmd(CMD_MASTER_ACTIVATION);
      wait();

      setWindow(0, 0, WIDTH - 1, HEIGHT - 1);
      setCursor(0, 0);
   }

   //! Put display to sleep
   void sleep()
   {
      sendCmd(CMD_DISP_UPDATE_CTRL2, /* disable clock signal */ 0x01);

      usleep(100000);
   }

   // chip select (CS) timing
   static const unsigned T_CSSUP = 2; // (us) Setup time before SCLK rise (Doc says 60 nS)
   static const unsigned T_CSHLD = 2; // (us) Hold time after SCLK fall (Doc says 65 nS)

   // SPI commands
   const uint8_t CMD_DRV_OUT_CTRL         = 0x01;
   const uint8_t CMD_GATE_DRV_V           = 0x03;
   const uint8_t CMD_SRC_DRV_V            = 0x04;
   const uint8_t CMD_DATA_ENTRY_MODE      = 0x11;
   const uint8_t CMD_SWRST                = 0x12;
   const uint8_t CMD_TEMP_SENSE_CTRL      = 0x18;
   const uint8_t CMD_MASTER_ACTIVATION    = 0x20;
   const uint8_t CMD_DISP_UPDATE_CTRL1    = 0x21;
   const uint8_t CMD_DISP_UPDATE_CTRL2    = 0x22;
   const uint8_t CMD_WRITE_RAM_BW         = 0x24;
   const uint8_t CMD_WRITE_RAM_RED        = 0x26;
   const uint8_t CMD_VCOM                 = 0x2C;
   const uint8_t CMD_LUT                  = 0x32;
   const uint8_t CMD_BRDR_WAVFRM          = 0x3C;
   const uint8_t CMD_RAM_X_ADDR_START_END = 0x44;
   const uint8_t CMD_RAM_Y_ADDR_START_END = 0x45;
   const uint8_t CMD_RAM_X_ADDR_CNTR      = 0x4E;
   const uint8_t CMD_RAM_Y_ADDR_CNTR      = 0x4F;

   static const unsigned SPI_CLOCK = 4000000; // 4 MHz

   MTL::Digital::Out<MTL::PIN_11> pin_dc;
   MTL::Digital::Out<MTL::PIN_12> pin_cs;
   MTL::Digital::Out<MTL::PIN_16> pin_rst;
   MTL::Digital::In<MTL::PIN_17>  pin_busy;
   MTL::Spi1                      spi{SPI_CLOCK, /* use_cs */ false, /* use_miso */ false};
};

}
