//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

//! \file Frame.h
//! \brief Platform abstraction interface for 2D rendering on a frame buffer

#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "PLT/Frame.h"
#include "GUI/Canvas.h"

namespace GUI {

const uint32_t RESIZABLE   = PLT::Frame::RESIZABLE;
const uint32_t NO_BORDER   = PLT::Frame::NO_BORDER;
const uint32_t FULL_SCREEN = PLT::Frame::FULL_SCREEN;

//! A frame buffer with 2D rendering primitive support
class Frame : public Canvas
{
private:
   PLT::Frame frame;

   // Implement GUI::Canvas
   virtual STB::Colour canvasGetPixel(signed x, signed y) const override
   {
      return frame.getPixel(x, y);
   }

protected:
   virtual const PLT::Image* canvasGetImage() const override
   {
      return &frame;
   }

private:
   virtual void canvasResize(unsigned width, unsigned height) override
   {
      frame.resize(width, height);
   }

   virtual void canvasRefresh(signed x1, signed y1, signed x2, signed y2) override
   {
      frame.refresh();
   }

   virtual void canvasClear(STB::Colour colour) override
   {
      frame.clear(colour);
   }

   virtual void canvasPoint(STB::Colour colour, signed x, signed y) override
   {
      frame.point(colour, x, y);
   }

   virtual void canvasSpan(STB::Colour colour, signed x1, signed y, signed x2) override
   {
      frame.span(colour, x1, y, x2);
   }

   virtual void canvasBlit(const Canvas& source,
                           unsigned x, unsigned y,
                           unsigned w, unsigned h,
                           unsigned src_x, unsigned src_y) override
   {
      const PLT::Image* image = source.canvasGetImage();
      if (image == nullptr)
      {
         Canvas::canvasBlit(source, x, y, w, h, src_x, src_y);
         return;
      }

      frame.blit(*image, x, y, w, h, src_x, src_y);
   }

public:
   Frame(const char* title_, unsigned width_ = 0, unsigned height_ = 0, uint32_t flags_ = 0)
      : GUI::Canvas(width_, height_)
      , frame(title_, width_, height_, flags_)
   {}
};

} // namespace GUI

#endif // GUI_FRAME_H
