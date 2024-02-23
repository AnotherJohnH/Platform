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
//
// \brief LCD driver for Sitronix ST7789V
//        240 x 320 dot 262K RGB color with frame memory
//        single-chip TFT controller

#pragma once

#include <initializer_list>
#include <unistd.h>

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/CanvasRGB565.h"

#include "MTL/rp2040/Gpio.h"
#include "MTL/rp2040/Pwm.h"

namespace MTL {

class Lcd_ST7789V
{
public:
   using Canvas = CanvasRGB565<Lcd_ST7789V>;

   //! Display width (pixels)
   static constexpr unsigned getWidth() { return WIDTH; }

   //! Display frame stride (pixels)
   static constexpr unsigned getStride() { return WIDTH; }

   //! Display height (pixels)
   static constexpr unsigned getHeight() { return HEIGHT; }

   Lcd_ST7789V()
   {
      out_cs = 1;
      out_dc = 1;
      out_wr = 1;
      out_rd = 1;
      out_db = 0;
      out_bl = 0;

      command({SWRESET});

      usleep(150000);

      command(TEON);
      command(COLMOD,   {0x05});
      command(PORCTRL,  {0x0c, 0x0c, 0x00, 0x33, 0x33});
      command(LCMCTRL,  {0x2c});
      command(VDVVRHEN, {0x01});
      command(VRHS,     {0x12});
      command(VDVS,     {0x20});
      command(PWCTRL1,  {0xa4, 0xa1});
      command(FRCTRL2,  {0x0f});

      command(GCTRL,    {0x14});
      command(VCOMS,    {0x37});
      command(GMCTRP1,  {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23});
      command(GMCTRN1,  {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23});

      command(INVON);
      command(SLPOUT);
      command(DISPON);

      usleep(150000);

      uint16_t       word_pair[2];
      const uint8_t* b = (uint8_t*)word_pair;

      word_pair[0] = 0;
      word_pair[1] = WIDTH - 1;
      command(CASET, {b[1], b[0], b[3], b[2]});

      word_pair[0] = 0;
      word_pair[1] = HEIGHT - 1;
      command(RASET, {b[1], b[0], b[3], b[2]});

      command(MADCTL, {MADCTL_COL_ORDER | MADCTL_SWAP_XY | MADCTL_SCAN_ORDER});

      usleep(50000);

      setBrightness(128);
   }

   void setBrightness(uint8_t level)
   {
      static const uint16_t table_gamma_8[256] =
      {
         0x0000, 0x0000, 0x0000, 0x0000, 0x0001, 0x0001, 0x0002, 0x0003,
         0x0004, 0x0006, 0x0008, 0x000a, 0x000d, 0x0010, 0x0013, 0x0018,
         0x001c, 0x0021, 0x0027, 0x002e, 0x0035, 0x003c, 0x0045, 0x004e,
         0x0058, 0x0062, 0x006e, 0x007a, 0x0087, 0x0095, 0x00a4, 0x00b3,
         0x00c4, 0x00d6, 0x00e8, 0x00fc, 0x0111, 0x0127, 0x013d, 0x0155,
         0x016e, 0x0189, 0x01a4, 0x01c1, 0x01de, 0x01fe, 0x021e, 0x023f,
         0x0262, 0x0287, 0x02ac, 0x02d3, 0x02fc, 0x0326, 0x0351, 0x037e,
         0x03ac, 0x03dc, 0x040d, 0x0440, 0x0474, 0x04aa, 0x04e2, 0x051b,
         0x0556, 0x0593, 0x05d1, 0x0611, 0x0653, 0x0696, 0x06dc, 0x0723,
         0x076c, 0x07b7, 0x0803, 0x0852, 0x08a2, 0x08f5, 0x0949, 0x099f,
         0x09f8, 0x0a52, 0x0aae, 0x0b0d, 0x0b6d, 0x0bd0, 0x0c34, 0x0c9b,
         0x0d04, 0x0d6f, 0x0ddc, 0x0e4c, 0x0ebe, 0x0f32, 0x0fa8, 0x1020,
         0x109b, 0x1118, 0x1198, 0x121a, 0x129e, 0x1325, 0x13ae, 0x1439,
         0x14c7, 0x1558, 0x15eb, 0x1680, 0x1718, 0x17b3, 0x1850, 0x18f0,
         0x1992, 0x1a37, 0x1adf, 0x1b89, 0x1c36, 0x1ce5, 0x1d98, 0x1e4d,
         0x1f05, 0x1fc0, 0x207d, 0x213d, 0x2200, 0x22c6, 0x238f, 0x245b,
         0x252a, 0x25fb, 0x26d0, 0x27a7, 0x2882, 0x295f, 0x2a40, 0x2b23,
         0x2c0a, 0x2cf3, 0x2de0, 0x2ed0, 0x2fc3, 0x30b9, 0x31b2, 0x32af,
         0x33ae, 0x34b1, 0x35b7, 0x36c1, 0x37cd, 0x38dd, 0x39f1, 0x3b07,
         0x3c21, 0x3d3e, 0x3e5f, 0x3f83, 0x40aa, 0x41d5, 0x4303, 0x4435,
         0x456a, 0x46a3, 0x47df, 0x491f, 0x4a62, 0x4ba9, 0x4cf4, 0x4e42,
         0x4f94, 0x50e9, 0x5242, 0x539f, 0x54ff, 0x5663, 0x57cb, 0x5936,
         0x5aa6, 0x5c19, 0x5d90, 0x5f0a, 0x6089, 0x620b, 0x6391, 0x651c,
         0x66aa, 0x683b, 0x69d1, 0x6b6b, 0x6d09, 0x6eaa, 0x7050, 0x71fa,
         0x73a8, 0x7559, 0x770f, 0x78c9, 0x7a87, 0x7c4a, 0x7e10, 0x7fda,
         0x81a9, 0x837c, 0x8553, 0x872e, 0x890d, 0x8af1, 0x8cd9, 0x8ec5,
         0x90b6, 0x92ab, 0x94a4, 0x96a1, 0x98a3, 0x9aa9, 0x9cb4, 0x9ec3,
         0xa0d7, 0xa2ef, 0xa50b, 0xa72c, 0xa952, 0xab7b, 0xadaa, 0xafdd,
         0xb214, 0xb451, 0xb691, 0xb8d7, 0xbb21, 0xbd6f, 0xbfc3, 0xc21b,
         0xc477, 0xc6d9, 0xc93f, 0xcbaa, 0xce19, 0xd08e, 0xd307, 0xd585,
         0xd807, 0xda8f, 0xdd1c, 0xdfad, 0xe243, 0xe4de, 0xe77e, 0xea23,
         0xeccd, 0xef7c, 0xf230, 0xf4e9, 0xf7a7, 0xfa6a, 0xfd32, 0xffff
      };

      out_bl = table_gamma_8[level];
   }

   void clear(uint16_t rgb_ = 0x0000)
   {
      command(RAMWR);

      out_cs = 0;

      for(unsigned i = 0; i < WIDTH * HEIGHT; ++i)
      {
         write(rgb_ >> 8);
         write(rgb_ & 0xFF);
      }

      out_cs = 1;
   }

   void display(const uint16_t* buffer_)
   {
      command(RAMWR);

      out_cs = 0;

      for(unsigned i = 0; i < WIDTH * HEIGHT; ++i)
      {
         write(buffer_[i] >> 8);
         write(buffer_[i] & 0xFF);
      }

      out_cs = 1;
   }

   static const unsigned WIDTH  = 320;
   static const unsigned HEIGHT = 240;

private:
   //! Write a single byte
   void write(uint8_t data)
   {
      out_wr = 0;
      out_db = data;
      out_wr = 1;
   }

   //! Write a command with a variable length parameter list
   void command(uint8_t cmd, std::initializer_list<uint8_t> data)
   {
      out_cs = 0;
      out_dc = 0;

      write(cmd);

      out_dc = 1;

      for(const auto byte : data) 
      {
         write(byte);
      }

      out_cs = 1;
   }

   //! Write a command with no parameters
   void command(uint8_t cmd)
   {
      command(cmd, {});
   }

   // Command codes
   static const uint8_t SWRESET  = 0x01;
   static const uint8_t SLPOUT   = 0x11;
   static const uint8_t INVOFF   = 0x20;
   static const uint8_t INVON    = 0x21;
   static const uint8_t GAMSET   = 0x26;
   static const uint8_t DISPOFF  = 0x28;
   static const uint8_t DISPON   = 0x29;
   static const uint8_t CASET    = 0x2A;
   static const uint8_t RASET    = 0x2B;
   static const uint8_t RAMWR    = 0x2C;
   static const uint8_t TEOFF    = 0x34;
   static const uint8_t TEON     = 0x35;
   static const uint8_t MADCTL   = 0x36;
   static const uint8_t COLMOD   = 0x3A;
   static const uint8_t PORCTRL  = 0xB2;
   static const uint8_t GCTRL    = 0xB7;
   static const uint8_t VCOMS    = 0xBB;
   static const uint8_t LCMCTRL  = 0xC0;
   static const uint8_t VDVVRHEN = 0xC2;
   static const uint8_t VRHS     = 0xC3;
   static const uint8_t VDVS     = 0xC4;
   static const uint8_t FRCTRL2  = 0xC6;
   static const uint8_t PWMFRSEL = 0xCC;
   static const uint8_t PWCTRL1  = 0xD0;
   static const uint8_t GMCTRP1  = 0xE0;
   static const uint8_t GMCTRN1  = 0xE1;

   // MADCTL command masks
   static const uint8_t MADCTL_ROW_ORDER   = 0b10000000;
   static const uint8_t MADCTL_COL_ORDER   = 0b01000000;
   static const uint8_t MADCTL_SWAP_XY     = 0b00100000;
   static const uint8_t MADCTL_SCAN_ORDER  = 0b00010000;
   static const uint8_t MADCTL_RGB_BGR     = 0b00001000;
   static const uint8_t MADCTL_HORIZ_ORDER = 0b00000100;

   MTL::Gpio::Out<1,MTL::PIN_14> out_cs; // Chip Select
   MTL::Gpio::Out<1,MTL::PIN_15> out_dc; // Data not Command
   MTL::Gpio::Out<1,MTL::PIN_16> out_wr; // Write
   MTL::Gpio::Out<1,MTL::PIN_17> out_rd; // Read
   MTL::Gpio::Out<8,MTL::PIN_19> out_db; // Data byte

   MTL::Pwm<MTL::PIN_4> out_bl{0x10000}; // Back-light
};

} // namespace MTL
