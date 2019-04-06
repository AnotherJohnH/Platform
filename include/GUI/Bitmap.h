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

//! \file Bitmap.h
//! \brief Platform abstraction interface for 2D rendering on a bitmap buffer

#ifndef GUI_BITMAP_H
#define GUI_BITMAP_H

#include "PLT/Bitmap.h"

#include "GUI/Canvas.h"

namespace GUI {

//!
class Bitmap : public Canvas
{
private:
   PLT::Bitmap bitmap;

   // Implement GUI::Canvas
   virtual STB::Colour canvasGetPixel(int32_t x, int32_t y) const override
   {
      return bitmap.getPixel(x, y);
   }

protected:
   virtual const PLT::Image* canvasGetImage() const override { return &bitmap; }

private:
   virtual void canvasResize(uint32_t width, uint32_t height) override {}

   virtual void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) override {}

   virtual void canvasClear(STB::Colour colour) override
   {
      bitmap.clear(colour);
   }

   virtual void canvasPoint(STB::Colour colour, int32_t x, int32_t y) override
   {
      bitmap.point(colour, x, y);
   }

   virtual void canvasSpan(STB::Colour colour, int32_t x1, int32_t y, int32_t x2) override
   {
      bitmap.span(colour, x1, y, x2);
   }

   virtual void canvasBlit(const Canvas& source,
                           uint32_t x, uint32_t y,
                           uint32_t w, uint32_t h,
                           uint32_t src_x, uint32_t src_y) override
   {
      const PLT::Image* image = source.canvasGetImage();
      if(image == nullptr)
      {
         Canvas::canvasBlit(source, x, y, w, h, src_x, src_y);
         return;
      }

      bitmap.blit(*image, x, y, w, h, src_x, src_y);
   }

   bool readPNG(const char* filename);

public:
   Bitmap(const char* filename)
   {
      if (readPNG(filename))
      {
         resize(bitmap.getWidth(), bitmap.getHeight());
      }
   }

   Bitmap(unsigned width, unsigned height)
   {
      resize(width, height);
   }

   virtual ~Bitmap() {}

   const PLT::Bitmap& getBitmap() const { return bitmap; }
};

} // namespace GUI

#endif // GUI_BITMAP_H
