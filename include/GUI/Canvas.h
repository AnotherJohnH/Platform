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

   Canvas(unsigned width_, unsigned height_)
      : size(width_, height_)
   {
   }

   //! Get canvas width (pixels)
   unsigned getWidth() const { return size.x; }

   //! Get canvas height (pixels)
   unsigned getHeight() const { return size.y; }

   //! Test pixel co-ordinates are inside the canvass boundaries
   bool isVisible(signed x, signed y) const
   {
      return (x >= 0) && (x < signed(getWidth())) && (y >= 0) && (y < signed(getHeight()));
   }

   //! Get colour of single pixel in the frame buffer
   STB::Colour getPixel(signed x, signed y) const
   {
      if(!isVisible(x, y)) return GUI::HIDDEN;

      return canvasGetPixel(x, y);
   }

   //! Set a single pixel
   void drawPoint(STB::Colour colour, signed x, signed y)
   {
      if(isVisible(x, y))
      {
         canvasPoint(colour, x, y);
      }
   }

   //! Set a horizontal row of pixels
   void drawSpan(STB::Colour colour, signed x1, signed y, signed x2)
   {
      clipAndSort(x1, x2, size.x);

      if((y >= 0) && (y < signed(size.y)))
      {
         canvasSpan(colour, x1, y, x2);
      }
   }

   //! Draw a straight line between two points
   void drawLine(STB::Colour colour, signed x1, signed y1, signed x2, signed y2)
   {
      // TODO clip to frame and then use canvasPoint() directly

      signed dx = x2 - x1;
      signed dy = y2 - y1;

      if(abs(dx) >= abs(dy))
      {
         signed ix = dx < 0 ? -1 : +1;
         signed iy = (dy << 16) / abs(dx);
         signed y  = y1 << 16;

         for(signed x = x1; x != x2; x += ix)
         {
            drawPoint(colour, x, y >> 16);
            y += iy;
         }
      }
      else
      {
         signed iy = dy < 0 ? -1 : +1;
         signed ix = (dx << 16) / abs(dy);
         signed x  = x1 << 16;

         for(signed y = y1; y != y2; y += iy)
         {
            drawPoint(colour, x >> 16, y);
            x += ix;
         }
      }
   }

   //! Draw a rectangle outline
   void drawRect(STB::Colour colour, signed x1, signed y1, signed x2, signed y2)
   {
      drawLine(colour, x1, y1, x2, y1);
      drawLine(colour, x2, y1, x2, y2);
      drawLine(colour, x2, y2, x1, y2);
      drawLine(colour, x1, y2, x1, y1);
   }

   //! Fill a rectangle
   void fillRect(STB::Colour colour, signed x1, signed y1, signed x2, signed y2)
   {
      clipAndSort(x1, x2, size.x);
      clipAndSort(y1, y2, size.y);

      for(signed y = y1; y < y2; y++)
      {
         canvasSpan(colour, x1, y, x2);
      }
   }

   //! Fill a triangle
   void fillTriangle(STB::Colour colour, signed x1, signed y1, signed x2, signed y2, signed x3,
                     signed y3)
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

      signed dxe = ((x3 - x1) << 16) / (y3 - y1);
      signed dxs;

      signed y  = y1;
      signed xs = x1 << 16;
      signed xe;

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
   void drawCircle(STB::Colour colour, signed cx, signed cy, unsigned r)
   {
      drawPoint(colour, cx, cy + r);
      drawPoint(colour, cx, cy - r);
      drawPoint(colour, cx + r, cy);
      drawPoint(colour, cx - r, cy);

      signed f     = 1 - r;
      signed ddf_x = 1;
      signed ddf_y = -2 * r;
      signed y     = r;

      for(signed x = 0; x < y;)
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
   void fillCircle(STB::Colour colour, signed cx, signed cy, unsigned r)
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

   void drawCurve(signed x1, signed y1, signed x2, signed y2, signed cx1, signed cy1, signed cx2,
                  signed cy2)
   {
      // TODO implementation
      assert(!"no implementation");
   }

   //! Draw an alphamap
   template <unsigned BPP>
   void drawAlphaMap(STB::Colour fg, STB::Colour bg, signed x, signed y, unsigned w, unsigned h,
                     const uint8_t* alphamap)
   {
      unsigned shift;
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

      for(unsigned v = 0; v < h; v++)
      {
         uint8_t byte       = *alphamap++;
         uint8_t alpha      = byte >> (8 - BPP);
         uint8_t last_alpha = alpha;
         signed  x1         = x;
         signed  x2         = x1;

         for(unsigned u = 0; true;)
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
   unsigned drawChar(STB::Colour fg, STB::Colour bg, signed x, signed y, const Font* font, uint8_t ch)
   {
      unsigned w = font->getWidth();

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
   void drawText(STB::Colour fg, STB::Colour bg, signed x, signed y, const Font* font, const char* s)
   {
      assert(s);

      while(*s)
      {
         x += drawChar(fg, bg, x, y, font, *s++);
      }
   }

   //! Blit from another canvas into this canvas
   void drawImage(const Canvas& source,
                  unsigned x, unsigned y,
                  unsigned w, unsigned h,
                  unsigned src_x, unsigned src_y)
   {
      canvasBlit(source, x, y, w, h, src_x, src_y);
   }

   //! Fill the entire canvas
   void clear(STB::Colour colour) { canvasClear(colour); }

   //! Refresh the whole canvas to the display device
   void refresh() { canvasRefresh(0, 0, size.x, size.y); }

   //! Resize the canvas
   void resize(unsigned width, unsigned height)
   {
      assert((width != 0) && (height != 0));

      size.x = width;
      size.y = height;

      canvasResize(size.x, size.y);
   }

private:
   static const unsigned MAX_ALPHA_BPP = 4;

   Vector size;
   STB::Colour alpha_table[1 << MAX_ALPHA_BPP] = {};

   //! Clip value to between 0 and limit (inclusive range)
   void clip(signed& value, unsigned limit)
   {
      if(value < 0)
         value = 0;
      else if(value > signed(limit))
         value = limit;
   }

   //! Clip two values to between 0 and limit (inclusive range) and
   //  ensure swap the values if the first is greater than the second
   void clipAndSort(signed& value1, signed& value2, unsigned limit)
   {
      clip(value1, limit);
      clip(value2, limit);

      if(value2 < value1)
      {
         std::swap(value1, value2);
      }
   }

   //! Compute alpha LUT for rendering alpha maps
   void preComputeAlpha(STB::Colour fg_, STB::Colour bg_, unsigned bpp)
   {
      assert(bpp <= MAX_ALPHA_BPP);

      unsigned max_alpha = (1 << bpp) - 1;

      if((alpha_table[0] != bg_) || (alpha_table[max_alpha] != fg_))
      {
         // Not already computed

         STB::ColourDecode fg(fg_);
         STB::ColourDecode bg(bg_);

         alpha_table[0]         = bg_;
         alpha_table[max_alpha] = fg_;

         for(unsigned alpha = 1; alpha < max_alpha; ++alpha)
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
   virtual STB::Colour canvasGetPixel(signed x, signed y) const { return GUI::HIDDEN; }

public:
   //! Get pointer to underlying frame buffer
   virtual const PLT::Image* canvasGetImage() const { return nullptr; }

private:
   //! Resize the frame buffer
   virtual void canvasResize(unsigned width, unsigned height) { assert(!"no implementation"); }

   //! Refresh a rectangular region of the frame buffer
   virtual void canvasRefresh(signed x1, signed y1, signed x2, signed y2) {}

   //! Clear frame buffer to a single colour
   virtual void canvasClear(STB::Colour colour)
   {
      for(signed y = 0; y < size.y; y++)
      {
         canvasSpan(colour, 0, y, size.x);
      }
   }

   //! Set a single pixel in the frame buffer
   virtual void canvasPoint(STB::Colour colour, signed x, signed y) = 0;

   //! Set a horizontal row of pixels in the frame buffer
   virtual void canvasSpan(STB::Colour colour, signed x1, signed y, signed x2)
   {
      assert(x1 < x2);

      for(signed x = x1; x < x2; x++)
      {
         canvasPoint(colour, x, y);
      }
   }

protected:
   //! Blit from another canvas into this canvas
   virtual void canvasBlit(const Canvas& source,
                           unsigned x, unsigned y,
                           unsigned w, unsigned h,
                           unsigned src_x, unsigned src_y)
   {
      assert((src_x + w) <= source.getWidth());
      assert((src_y + h) <= source.getHeight());

      for(unsigned u = 0; u < w; u++)
      {
         for(unsigned v = 0; v < h; v++)
         {
            canvasPoint(source.getPixel(src_x + u, src_y + v), x + u, y + v);
         }
      }
   }
};

} // namespace GUI

#endif
