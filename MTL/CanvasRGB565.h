//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Generic canvas for displays with 16-bit RGB 565 pixels

#pragma once

#include "GUI/Canvas.h"

namespace MTL {

template <typename DISPLAY>
class CanvasRGB565 : public GUI::Canvas
{
public:
   CanvasRGB565()
      : GUI::Canvas(DISPLAY::getWidth(), DISPLAY::getHeight())
   {
   }

   DISPLAY hw{};

private:
   void canvasPoint(STB::Colour colour, int32_t x_, int32_t y_) override
   {
      unsigned red = (colour >> 19) &  0b11111;
      unsigned grn = (colour >> 10) & 0b111111;
      unsigned blu = (colour >>  3) &  0b11111;

      uint16_t rgb565 = (red << 11) | (grn << 5) | blu;

      frame[y_ * DISPLAY::getStride() + x_] = rgb565;
   }

   void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) override
   {
      hw.display(frame);
   }

   uint16_t frame[DISPLAY::getStride() * DISPLAY::getHeight()];
};

} // namespace MTL
