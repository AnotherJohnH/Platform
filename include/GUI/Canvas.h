//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef GUI_CANVAS_H
#define GUI_CANVAS_H

#include <cassert>
#include <cstdlib>
#include <utility>

#include "GUI/Colour.h"
#include "GUI/Font.h"
#include "GUI/Vector.h"

namespace PLT { class Image; }

namespace GUI {

//! Abstract drawable class
class Canvas
{
public:
   Canvas()
      : size(0, 0)
   {
   }

   Canvas(uint32_t width_, uint32_t height_)
      : size(width_, height_)
   {
   }

   //! Get canvas width (pixels)
   uint32_t getWidth() const { return size.x; }

   //! Get canvas height (pixels)
   uint32_t getHeight() const { return size.y; }

   //! Test pixel co-ordinates are inside the canvass boundaries
   bool isVisible(int32_t x, int32_t y) const
   {
      return (x >= 0) && (x < int32_t(getWidth())) && (y >= 0) && (y < int32_t(getHeight()));
   }

   //! Get colour of single pixel in the frame buffer
   STB::Colour getPixel(int32_t x, int32_t y) const
   {
      if(!isVisible(x, y)) return GUI::HIDDEN;

      return canvasGetPixel(x, y);
   }

   //! Set a single pixel
   void drawPoint(STB::Colour colour, int32_t x, int32_t y)
   {
      if(isVisible(x, y))
      {
         canvasPoint(colour, x, y);
      }
   }

   //! Set a horizontal row of pixels
   void drawSpan(STB::Colour colour, int32_t x1, int32_t y, int32_t x2)
   {
      clipAndSort(x1, x2, size.x);

      if((y >= 0) && (y < int32_t(size.y)))
      {
         canvasSpan(colour, x1, y, x2);
      }
   }

   //! Draw a straight line between two points
   void drawLine(STB::Colour colour, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      // TODO clip to frame and then use canvasPoint() directly

      int32_t dx = x2 - x1;
      int32_t dy = y2 - y1;

      if(abs(dx) > abs(dy))
      {
         int32_t ix = dx < 0 ? -1 : +1;
         int32_t iy = (dy << 16) / abs(dx);
         int32_t y  = y1 << 16;

         for(int32_t x = x1; x != x2; x += ix)
         {
            drawPoint(colour, x, y >> 16);
            y += iy;
         }
      }
      else if(dy != 0)
      {
         int32_t iy = dy < 0 ? -1 : +1;
         int32_t ix = (dx << 16) / abs(dy);
         int32_t x  = x1 << 16;

         for(int32_t y = y1; y != y2; y += iy)
         {
            drawPoint(colour, x >> 16, y);
            x += ix;
         }
      }
   }

   //! Draw a rectangle outline
   void drawRect(STB::Colour colour, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      drawLine(colour, x1, y1, x2, y1);
      drawLine(colour, x2, y1, x2, y2);
      drawLine(colour, x2, y2, x1, y2);
      drawLine(colour, x1, y2, x1, y1);
   }

   //! Fill a rectangle
   void fillRect(STB::Colour colour, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      clipAndSort(x1, x2, size.x);
      clipAndSort(y1, y2, size.y);

      for(int32_t y = y1; y < y2; y++)
      {
         canvasSpan(colour, x1, y, x2);
      }
   }

   //! Fill a triangle
   void fillTriangle(STB::Colour colour, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3,
                     int32_t y3)
   {
      if(y2 < y1)
      {
         std::swap(y2, y1);
         std::swap(x2, x1);
      }

      if(y3 < y1)
      {
         std::swap(y3, y1);
         std::swap(x3, x1);
      }

      if(y3 < y2)
      {
         std::swap(y3, y2);
         std::swap(x3, x2);
      }

      if(y3 == y1) return;

      int32_t dxe = ((x3 - x1) << 16) / (y3 - y1);
      int32_t dxs;

      int32_t y  = y1;
      int32_t xs = x1 << 16;
      int32_t xe;

      if(y2 > y1)
      {
         xe = x1 << 16;

         dxs = ((x2 - x1) << 16) / (y2 - y1);

         for(; y < y2; ++y)
         {
            if(xs < xe)
               drawSpan(colour, xs >> 16, y, xe >> 16);
            else
               drawSpan(colour, xe >> 16, y, xs >> 16);

            xs += dxs;
            xe += dxe;
         }
      }
      else
      {
         xe = x2 << 16;
      }

      if(y3 == y2) return;

      dxs = ((x3 - x2) << 16) / (y3 - y2);

      for(; y < y3; ++y)
      {
         if(xs < xe)
            drawSpan(colour, xs >> 16, y, xe >> 16);
         else
            drawSpan(colour, xe >> 16, y, xs >> 16);

         xs += dxs;
         xe += dxe;
      }
   }

   //! Draw a circle
   void drawCircle(STB::Colour colour, int32_t cx, int32_t cy, uint32_t r)
   {
      drawPoint(colour, cx, cy + r);
      drawPoint(colour, cx, cy - r);
      drawPoint(colour, cx + r, cy);
      drawPoint(colour, cx - r, cy);

      int32_t f     = 1 - r;
      int32_t ddf_x = 1;
      int32_t ddf_y = -2 * r;
      int32_t y     = r;

      for(int32_t x = 0; x < y;)
      {
         if(f >= 0)
         {
            y--;
            ddf_y += 2;
            f += ddf_y;
         }
         x++;
         ddf_x += 2;
         f += ddf_x;

         drawPoint(colour, cx + x, cy + y);
         drawPoint(colour, cx - x, cy + y);

         drawPoint(colour, cx + x, cy - y);
         drawPoint(colour, cx - x, cy - y);

         drawPoint(colour, cx + y, cy + x);
         drawPoint(colour, cx - y, cy + x);

         drawPoint(colour, cx + y, cy - x);
         drawPoint(colour, cx - y, cy - x);
      }
   }

   //! Fill a circle
   void fillCircle(STB::Colour colour, int32_t cx, int32_t cy, uint32_t r)
   {
      drawSpan(colour, cx, cy + r, cx);
      drawSpan(colour, cx - r, cy, cx + r);
      drawSpan(colour, cx, cy - r, cx);

      int f     = 1 - r;
      int ddf_x = 1;
      int ddf_y = -2 * r;
      int y     = r;

      for(int x = 0; x < y;)
      {
         if(f >= 0)
         {
            y--;
            ddf_y += 2;
            f += ddf_y;
         }
         x++;
         ddf_x += 2;
         f += ddf_x;

         drawSpan(colour, cx - x, cy + y, cx + x);
         drawSpan(colour, cx - x, cy - y, cx + x);
         drawSpan(colour, cx - y, cy + x, cx + y);
         drawSpan(colour, cx - y, cy - x, cx + y);
      }
   }

   void drawCurve(STB::Colour colour,
                  int32_t x1,  int32_t y1,
                  int32_t x2,  int32_t y2,
                  int32_t cx1, int32_t cy1,
                  int32_t cx2, int32_t cy2)
   {
      int32_t ax =   -x1 + 3*cx1 - 3*cx2 + x2;
      int32_t ay =   -y1 + 3*cy1 - 3*cy2 + y2;

      int32_t bx =  3*x1 - 6*cx1 + 3*cx2;
      int32_t by =  3*y1 - 6*cy1 + 3*cy2;

      int32_t cx = -3*x1 + 3*cx1;
      int32_t cy = -3*y1 + 3*cy1;

      int32_t dx =    x1;
      int32_t dy =    y1;

      int32_t prev_x = x1;
      int32_t prev_y = y1;

      // TODO some kind of recursive sub-division would seem to be in order here
      for(double t = 0.1; t <= 1.0; t += 0.1)
      {
         int32_t x = ax * t*t*t + bx * t*t + cx * t + dx;
         int32_t y = ay * t*t*t + by * t*t + cy * t + dy;

         drawLine(colour, prev_x, prev_y, x, y);

         prev_x = x;
         prev_y = y;
      }
   }

   //! Draw an alphamap
   template <uint32_t BPP>
   void drawAlphaMap(STB::Colour fg, STB::Colour bg, int32_t x, int32_t y, uint32_t w, uint32_t h,
                     const uint8_t* alphamap)
   {
      uint32_t shift;
      if(BPP > MAX_ALPHA_BPP)
      {
         shift = BPP - MAX_ALPHA_BPP;
         preComputeAlpha(fg, bg, MAX_ALPHA_BPP);
      }
      else
      {
         shift = 0;
         preComputeAlpha(fg, bg, BPP);
      }

      for(uint32_t v = 0; v < h; v++)
      {
         uint8_t byte       = *alphamap++;
         uint8_t alpha      = byte >> (8 - BPP);
         uint8_t last_alpha = alpha;
         int32_t  x1         = x;
         int32_t  x2         = x1;

         for(uint32_t u = 0; true;)
         {
            if(alpha != last_alpha)
            {
               last_alpha >>= shift;
               if(last_alpha)
               {
                  drawSpan(alpha_table[last_alpha], x1, y + v, x2);
               }
               last_alpha = alpha;
               x1         = x2;
            }
            x2++;

            if(++u == w)
            {
               if(x1 != x2)
               {
                  alpha >>= shift;
                  if(alpha)
                  {
                     drawSpan(alpha_table[alpha], x1, y + v, x2);
                  }
               }
               break;
            }

            if((u % (8 / BPP)) == 0)
            {
               byte = *alphamap++;
            }
            else if(BPP < 8)
            {
               byte <<= BPP;
            }
            alpha = byte >> (8 - BPP);
         }
      }
   }

   //! Draw a single character from a font
   uint32_t drawChar(STB::Colour fg, STB::Colour bg, int32_t x, int32_t y, const Font* font, uint8_t ch)
   {
      uint32_t w = font->getWidth();

      const uint8_t* map = font->getAlphaMap(ch);
      if(map != nullptr)
      {
          switch(font->getBPP())
          {
          case 1: drawAlphaMap<1>(fg, bg, x, y, w, font->getHeight(), map); break;
          case 2: drawAlphaMap<2>(fg, bg, x, y, w, font->getHeight(), map); break;
          case 4: drawAlphaMap<4>(fg, bg, x, y, w, font->getHeight(), map); break;
          case 8: drawAlphaMap<8>(fg, bg, x, y, w, font->getHeight(), map); break;

          default: break;
          }
       }

      return w;
   }

   //! Draw a string of text
   void drawText(STB::Colour fg, STB::Colour bg, int32_t x, int32_t y, const Font* font, const char* s)
   {
      assert(s);

      while(*s)
      {
         x += drawChar(fg, bg, x, y, font, *s++);
      }
   }

   //! Blit from another canvas into this canvas
   void drawImage(const Canvas& source,
                  uint32_t x, uint32_t y,
                  uint32_t w = 0, uint32_t h = 0,
                  uint32_t src_x = 0, uint32_t src_y = 0)
   {
      if (w == 0) w = source.getWidth();
      if (h == 0) h = source.getHeight();

      canvasBlit(source, x, y, w, h, src_x, src_y);
   }

   //! Fill the entire canvas
   void clear(STB::Colour colour) { canvasClear(colour); }

   //! Refresh the whole canvas to the display device
   void refresh() { canvasRefresh(0, 0, size.x, size.y); }

   //! Resize the canvas
   void resize(uint32_t width, uint32_t height)
   {
      assert((width != 0) && (height != 0));

      size.x = width;
      size.y = height;

      canvasResize(size.x, size.y);
   }

private:
   static const uint32_t MAX_ALPHA_BPP = 4;

   Vector size;
   STB::Colour alpha_table[1 << MAX_ALPHA_BPP] = {};

   //! Clip value to between 0 and limit (inclusive range)
   void clip(int32_t& value, uint32_t limit)
   {
      if(value < 0)
         value = 0;
      else if(value > int32_t(limit))
         value = limit;
   }

   //! Clip two values to between 0 and limit (inclusive range) and
   //  ensure swap the values if the first is greater than the second
   void clipAndSort(int32_t& value1, int32_t& value2, uint32_t limit)
   {
      clip(value1, limit);
      clip(value2, limit);

      if(value2 < value1)
      {
         std::swap(value1, value2);
      }
   }

   //! Compute alpha LUT for rendering alpha maps
   void preComputeAlpha(STB::Colour fg_, STB::Colour bg_, uint32_t bpp)
   {
      assert(bpp <= MAX_ALPHA_BPP);

      uint32_t max_alpha = (1 << bpp) - 1;

      if((alpha_table[0] != bg_) || (alpha_table[max_alpha] != fg_))
      {
         // Not already computed

         STB::ColourDecode fg(fg_);
         STB::ColourDecode bg(bg_);

         alpha_table[0]         = bg_;
         alpha_table[max_alpha] = fg_;

         for(uint32_t alpha = 1; alpha < max_alpha; ++alpha)
         {
            alpha_table[alpha] =
               STB::RGB((fg.red() * alpha + bg.red() * (max_alpha - alpha)) / max_alpha,
                        (fg.grn() * alpha + bg.grn() * (max_alpha - alpha)) / max_alpha,
                        (fg.blu() * alpha + bg.blu() * (max_alpha - alpha)) / max_alpha);
         }
      }
   }

   // Overide the following interface with a frame buffer implementation

   //! Get colour of single pixel in the frame buffer
   virtual STB::Colour canvasGetPixel(int32_t x, int32_t y) const { return GUI::HIDDEN; }

public:
   //! Get pointer to underlying frame buffer
   virtual const PLT::Image* canvasGetImage() const { return nullptr; }

private:
   //! Resize the frame buffer
   virtual void canvasResize(uint32_t width, uint32_t height) { assert(!"no implementation"); }

   //! Refresh a rectangular region of the frame buffer
   virtual void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {}

   //! Clear frame buffer to a single colour
   virtual void canvasClear(STB::Colour colour)
   {
      for(int32_t y = 0; y < size.y; y++)
      {
         canvasSpan(colour, 0, y, size.x);
      }
   }

   //! Set a single pixel in the frame buffer
   virtual void canvasPoint(STB::Colour colour, int32_t x, int32_t y) = 0;

   //! Set a horizontal row of pixels in the frame buffer
   virtual void canvasSpan(STB::Colour colour, int32_t x1, int32_t y, int32_t x2)
   {
      assert(x1 < x2);

      for(int32_t x = x1; x < x2; x++)
      {
         canvasPoint(colour, x, y);
      }
   }

protected:
   //! Blit from another canvas into this canvas
   virtual void canvasBlit(const Canvas& source,
                           uint32_t x, uint32_t y,
                           uint32_t w, uint32_t h,
                           uint32_t src_x, uint32_t src_y)
   {
      assert((src_x + w) <= source.getWidth());
      assert((src_y + h) <= source.getHeight());

      for(uint32_t u = 0; u < w; u++)
      {
         for(uint32_t v = 0; v < h; v++)
         {
            canvasPoint(source.getPixel(src_x + u, src_y + v), x + u, y + v);
         }
      }
   }
};

} // namespace GUI

#endif
