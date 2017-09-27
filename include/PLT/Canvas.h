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

#ifndef PLT_PAPER_H
#define PLT_PAPER_H

#include  "PLT/Frame.h"

#include  "GUI/Canvas.h"

namespace PLT {

//! GUI Canvas implementation
class Canvas : public GUI::Canvas
{
private:
   PLT::Frame     frame;

   // Implement GUI::Canvas
   virtual GUI::Colour paperGetPixel(signed x, signed y) const override
   {
      return frame.getPixel(x, y);
   }

   virtual void paperResize(unsigned width, unsigned height) override
   {
      frame.resize(width, height);
   }

   virtual void paperRefresh(signed x1, signed y1, signed x2, signed y2) override
   {
      frame.refresh();
   }

   virtual void paperPoint(GUI::Colour colour, signed x, signed y) override
   {
      frame.setPixel(x, y, colour);
   }

   virtual void paperSpan(GUI::Colour colour, signed x1, signed y, signed x2) override
   {
      for(signed x=x1; x<x2; x++)
      {
         frame.setPixel(x, y, colour);
      }
   }

public:
   Canvas(const char* title_, unsigned width_ = 0, unsigned height_ = 0, uint32_t flags_ = 0)
      : GUI::Canvas(width_, height_)
      , frame(title_, width_, height_, flags_)
   {}

   void blit(unsigned x, unsigned y, unsigned offset, unsigned width, const FrameBase& image)
   {
      frame.blit(x, y, offset, width, image);
   }
};

} // namespace PLT

#endif
