//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Display interface

#pragma once

#if defined(HW_DISPLAY_WAVESHARE_EPAPER)

#include "MTL/EPaper_WS2_13_V3.h"

#elif defined(HW_DISPLAY_BADGER2040)

#elif defined(HW_DISPLAY_TUFTY2040)

#elif defined(HW_DISPLAY_NATIVE)

#include "native/Panel.h"

#endif

namespace hw {

#if defined(HW_DISPLAY_WAVESHARE_EPAPER)

class Display : public MTL::EPaper_WS2_13_V3
{
public:
   Display()
   {
      MTL::config.gpio(PIN_DC,  ">(e-paper)  DC");
      MTL::config.gpio(PIN_CS,  ">(e-paper)  CS");
      MTL::config.gpio(PIN_RST, ">(e-paper) RST");
      MTL::config.gpio(PIN_BSY, "<(e-paper) BSY");

      MTL::config.gpio(SPI::PIN_CS,   ">(e-paper) SPI   CS");
      MTL::config.gpio(SPI::PIN_CLK,  ">(e-paper) SPI  CLK");
      MTL::config.gpio(SPI::PIN_MOSI, ">(e-paper) SPI MOSI");
      MTL::config.gpio(SPI::PIN_MISO, "<(e-paper) SPI MISO");
   }

   void setBrightness(uint8_t level_) {}
};

#elif defined(HW_DISPLAY_BADGER2040)

class Display : public MTL::badger2040::EPaper
{
public:
   Display()
   {
      MTL::config.gpio(MTL::badger2040::PIN_EPAPER_DC,  ">DC  (e-paper)");
      MTL::config.gpio(MTL::badger2040::PIN_EPAPER_CS,  ">CS  (e-paper)");
      MTL::config.gpio(MTL::badger2040::PIN_EPAPER_RST, ">RST (e-paper)");
      MTL::config.gpio(MTL::badger2040::PIN_EPAPER_BSY, "<BSY (e-paper)");

      MTL::config.gpio(MTL::badger2040::EPAPER_SPI::PIN_CS,   ">SPI CS   (e-paper)");
      MTL::config.gpio(MTL::badger2040::EPAPER_SPI::PIN_CLK,  ">SPI CLK  (e-paper)");
      MTL::config.gpio(MTL::badger2040::EPAPER_SPI::PIN_MOSI, ">SPI MOSI (e-paper)");
      MTL::config.gpio(MTL::badger2040::EPAPER_SPI::PIN_MISO, "<SPI MISO (e-paper)");
   }

   void setBrightness(uint8_t level_) {}
};

#elif defined(HW_DISPLAY_TUFTY2040)

class Display : public MTL::tufty2040::Lcd
{
public:
   Display()
   {
      MTL::config.gpio(MTL::IO_PIN_10,  ">CS (LCD)");
      MTL::config.gpio(MTL::IO_PIN_11,  ">RS (LCD)");
      MTL::config.gpio(MTL::IO_PIN_12,  ">WR (LCD)");
      MTL::config.gpio(MTL::IO_PIN_13,  ">RD (LCD)");

      MTL::config.gpio(MTL::IO_PIN_14,  "=DB0 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_15,  "=DB1 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_16,  "=DB2 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_17,  "=DB3 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_18,  "=DB4 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_19,  "=DB5 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_20,  "=DB6 (LCD)");
      MTL::config.gpio(MTL::IO_PIN_21,  "=DB7 (LCD)");
   }
};

#elif defined(HW_DISPLAY_NATIVE)

class Display
{
public:
   Display() = default;

   void setBrightness(uint8_t level_) {}

   static constexpr unsigned WIDTH  = HW_DISPLAY_WIDTH;
   static constexpr unsigned HEIGHT = HW_DISPLAY_HEIGHT;

   using Canvas = Panel<WIDTH, HEIGHT, /* SCALE */ 2, /* BORDER */ 16>;

   static constexpr unsigned getWidth() { return WIDTH; }
   static constexpr unsigned getHeight() { return HEIGHT; }

   unsigned var{0};
};

#endif

} // namespace hw
