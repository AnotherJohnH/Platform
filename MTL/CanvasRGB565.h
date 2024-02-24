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
