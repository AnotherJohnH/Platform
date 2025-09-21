//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Generic canvas for displays with monochrome 1-bit pixels

#pragma once

#include "GUI/Canvas.h"

namespace MTL {

template <typename DISPLAY, bool SWAP_XY = false, bool INVERT = false>
class CanvasMono1 : public GUI::Canvas
{
public:
   CanvasMono1()
      : GUI::Canvas(DISPLAY::getWidth(), DISPLAY::getHeight())
   {
   }

   void quickRefresh()
   {
      hw.display(frame, /* quick */ true);
   }

   DISPLAY hw{};

private:
   void canvasPoint(STB::Colour colour, int32_t x_, int32_t y_) override
   {
      int32_t  col  = SWAP_XY ? y_                            : x_;
      int32_t  row  = SWAP_XY ? DISPLAY::getWidth() - x_  - 1 : y_;
      uint8_t& byte = frame[row * DISPLAY::getStride() + (col / 8)];
      uint8_t  bit  = 0b10000000 >> (col % 8);

      if ((colour == STB::BLACK) != INVERT)
      {
         byte &= ~bit;
      }
      else
      {
         byte |= bit;
      }
   }

   void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) override
   {
      hw.display(frame);
   }

   uint8_t frame[DISPLAY::getStride() * (SWAP_XY ? DISPLAY::getWidth()
                                                 : DISPLAY::getHeight())];
};

} // namespace MTL
