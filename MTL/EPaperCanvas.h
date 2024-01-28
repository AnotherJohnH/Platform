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

//! \brief Generic canvas for E-Paper displays

#pragma once

#include "GUI/Canvas.h"

namespace MTL {

template <typename EPAPER, bool SWAP_XY = false>
class EPaperCanvas : public GUI::Canvas
{
public:
   EPaperCanvas()
      : GUI::Canvas(SWAP_XY ? EPAPER::getHeight() : EPAPER::getWidth(),
                    SWAP_XY ? EPAPER::getWidth()  : EPAPER::getHeight())
   {
   }

   void partialRefresh()
   {
      epaper.display(frame, /* partial */ true);
   }

private:
   void canvasPoint(STB::Colour colour, int32_t x_, int32_t y_) override
   {
      int32_t  x    = SWAP_XY ? y_                            : x_;
      int32_t  y    = SWAP_XY ? EPAPER::getHeight() - x_  - 1 : y_;
      uint8_t& byte = frame[y * EPAPER::getStride() + (x / 8)];
      uint8_t  bit  = 0b10000000 >> (x % 8);

      if (colour == STB::BLACK)
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
      epaper.display(frame);
   }

   EPAPER  epaper{};
   uint8_t frame[EPAPER::getStride() * EPAPER::getHeight()];
};

} // namespace MTL
