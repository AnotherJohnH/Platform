//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>

#include "STB/Colour.h"

#include "GUI/Font.h"
#include "GUI/Vector.h"

namespace PLT { class Image; }

namespace GUI {

const STB::Colour HIDDEN = STB::RGBA(0x00, 0x00, 0x00, 0xFF);

//! Abstract drawable class
class Canvas
{
public:
   Canvas() = default;

   Canvas(uint32_t width_, uint32_t height_)
      : size(width_, height_)
   {
   }

   virtual ~Canvas() = default;

   //! Get canvas width (pixels)
   int32_t getWidth() const { return size.x; }

   //! Get canvas height (pixels)
   int32_t getHeight() const { return size.y; }

   //! Test pixel co-ordinates are inside the canvas boundary
   bool isVisible(int32_t x, int32_t y) const
   {
      return (x >= 0) && (x < getWidth()) &&
             (y >= 0) && (y < getHeight());
   }

   //! Get colour of single pixel on the canvas
   STB::Colour getPixel(int32_t x, int32_t y) const
   {
      return isVisible(x, y) ? canvasGetPixel(x, y)
                             : HIDDEN;
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
      if((y >= 0) && (y < getHeight()))
      {
         clipAndSort(x1, x2, size.x);

         canvasSpan(colour, x1, y, x2);
      }
   }

   //! Draw a straight line between two points
   void drawLine(STB::Colour colour, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      // TODO clip to boundary and then use canvasPoint() directly

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
   void fillTriangle(STB::Colour colour,
                     int32_t x1, int32_t y1,
                     int32_t x2, int32_t y2,
                     int32_t x3, int32_t y3)
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

      int32_t y  = y1;

      int32_t xe = x1 << 16;
      int32_t dxe = ((x3 - x1) << 16) / (y3 - y1);

      int32_t xs;
      int32_t dxs;

      if(y2 > y1)
      {
         xs = x1 << 16;

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
         xs = x2 << 16;
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

   class Bezier
   {
   public:
      using Vector = STB::Vector2<int32_t>;

      Bezier(Canvas*      canvas_,
             STB::Colour  colour_,
             int32_t x1,  int32_t y1,
             int32_t x2,  int32_t y2,
             int32_t cx1, int32_t cy1,
             int32_t cx2, int32_t cy2)
         : canvas(canvas_)
         , colour(colour_)
      {
         a.x = -x1 + 3*cx1 - 3*cx2 + x2;
         a.y = -y1 + 3*cy1 - 3*cy2 + y2;

         b.x = 3*x1 - 6*cx1 + 3*cx2;
         b.y = 3*y1 - 6*cy1 + 3*cy2;

         c.x = -3*x1 + 3*cx1;
         c.y = -3*y1 + 3*cy1;

         d.x = x1;
         d.y = y1;
      }

      void drawCurve(double t1, const Vector& p1, double t2, const Vector& p2)
      {
         double t = (t1 + t2) * 0.5;
         Vector p = compute(t);

         Vector d1 = p - p1;
         if (d1.getSquare() > LIMIT_SQUARED)
         {
            drawCurve(t1, p1, t, p);
         }
         else
         {
            canvas->drawLine(colour, p1.x, p1.y, p.x, p.y);
         }

         Vector d2 = p2 - p;
         if (d2.getSquare() > LIMIT_SQUARED)
         {
            drawCurve(t, p, t2, p2);
         }
         else
         {
            canvas->drawLine(colour, p.x, p.y, p2.x, p2.y);
         }
      }

      void fillCurve(double t1, const Vector& p1, double t2, const Vector& p2)
      {
         double t = (t1 + t2) * 0.5;
         Vector p = compute(t);

         Vector d1 = p - p1;
         if (d1.getSquare() > LIMIT_SQUARED)
         {
            fillCurve(t1, p1, t, p);
         }
         else
         {
            canvas->fillTriangle(colour, d.x, d.y, p1.x, p1.y, p.x, p.y);
         }

         Vector d2 = p2 - p;
         if (d2.getSquare() > LIMIT_SQUARED)
         {
            fillCurve(t, p, t2, p2);
         }
         else
         {
            canvas->fillTriangle(colour, d.x, d.y, p.x, p.y, p2.x, p2.y);
         }
      }

   private:
      const int32_t LIMIT = 3;
      const int32_t LIMIT_SQUARED = LIMIT * LIMIT;

      Vector compute(double t)
      {
         return Vector{int32_t(a.x * t*t*t + b.x * t*t + c.x * t + d.x + 0.5),
                       int32_t(a.y * t*t*t + b.y * t*t + c.y * t + d.y + 0.5)};
      }

      Canvas*     canvas;
      STB::Colour colour;
      Vector      a, b, c, d;
   };

   //! Draw a bezier curve
   void drawCurve(STB::Colour colour,
                  int32_t x1,  int32_t y1,
                  int32_t x2,  int32_t y2,
                  int32_t cx1, int32_t cy1,
                  int32_t cx2, int32_t cy2)
   {
      Bezier bezier{this, colour, x1, y1, x2, y2, cx1, cy1, cx2, cy2};;

      Bezier::Vector start{x1, y1};
      Bezier::Vector end{x2, y2};

      bezier.drawCurve(0.0, start, 1.0, end);
   }

   //! Fill a bezier curve
   void fillCurve(STB::Colour colour,
                  int32_t x1,  int32_t y1,
                  int32_t x2,  int32_t y2,
                  int32_t cx1, int32_t cy1,
                  int32_t cx2, int32_t cy2)
   {
      Bezier bezier{this, colour, x1, y1, x2, y2, cx1, cy1, cx2, cy2};;

      Bezier::Vector start{x1, y1};
      Bezier::Vector end{x2, y2};

      bezier.fillCurve(0.0, start, 1.0, end);
   }

   //! Draw an alphamap
   template <uint32_t BPP>
   void drawAlphaMap(STB::Colour fg, STB::Colour bg,
                     int32_t x, int32_t y, uint32_t w, uint32_t h,
                     const uint8_t* alphamap)
   {
      uint32_t shift;
      if(BPP > LOG2_MAX_ALPHA_BPP)
      {
         shift = BPP - LOG2_MAX_ALPHA_BPP;
         preComputeAlpha(fg, bg, LOG2_MAX_ALPHA_BPP);
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
         int32_t x1         = x;
         int32_t x2         = x1;

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
   uint32_t drawChar(STB::Colour fg, STB::Colour bg,
                     int32_t x, int32_t y, const Font* font, uint8_t ch)
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
   void drawText(STB::Colour fg, STB::Colour bg,
                 int32_t x, int32_t y, const Font* font, const char* s)
   {
      assert(s);

      while(*s)
      {
         x += drawChar(fg, bg, x, y, font, *s++);
      }
   }

   //! Blit from another canvas into this canvas
   void drawImage(const Canvas& source,
                  int32_t x, int32_t y,
                  uint32_t w = 0, uint32_t h = 0,
                  int32_t src_x = 0, int32_t src_y = 0)
   {
      if(w == 0) w = source.getWidth();
      if(h == 0) h = source.getHeight();

      canvasBlit(source, x, y, w, h, src_x, src_y);
   }

   //! Fill the entire canvas
   void clear(STB::Colour colour) { canvasClear(colour); }

   //! Refresh the whole canvas to the display device
   void refresh() { canvasRefresh(0, 0, size.x, size.y); }

   //! Quick refresh (for ePaper support)
   virtual void quickRefresh() { refresh(); }

   //! Resize the canvas
   void resize(uint32_t width, uint32_t height)
   {
      assert((width != 0) && (height != 0));

      size.x = width;
      size.y = height;

      canvasResize(size.x, size.y);
   }

   //! Get pointer to underlying frame buffer
   const PLT::Image* getImage() const { return canvasGetImage(); }

   //! Draw a point
   void drawPoint(STB::Colour colour, const Position& p)
   {
      drawPoint(colour, p.x, p.y);
   }

   //! Draw a straight line between two points
   void drawLine(STB::Colour colour, const Position& p1, const Position& p2)
   {
      drawLine(colour, p1.x, p1.y, p2.x, p2.y);
   }

   //! Draw a bezier curve
   void drawCurve(STB::Colour colour,
                  const Position& p1,
                  const Position& p2,
                  const Position& cp1,
                  const Position& cp2)
   {
      drawCurve(colour, p1.x, p1.y, p2.x, p2.y, cp1.x, cp1.y, cp2.x, cp2.y);
   }

   //! Fill a triangle
   void fillTriangle(STB::Colour colour,
                     const Position& p1,
                     const Position& p2,
                     const Position& p3)
   {
      fillTriangle(colour, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
   }

   //! Fill a bezier curve
   void fillCurve(STB::Colour colour,
                  const Position& p1,
                  const Position& p2,
                  const Position& cp1,
                  const Position& cp2)
   {
      fillCurve(colour, p1.x, p1.y, p2.x, p2.y, cp1.x, cp1.y, cp2.x, cp2.y);
   }

   //! Draw a string of text
   void drawText(STB::Colour fg, STB::Colour bg, const Position& p,
                 const Font* font, const char* text)
   {
      drawText(fg, bg, p.x, p.y, font, text);
   }

protected:
   // Overide the following interface with a frame buffer implementation

   //! Set a single pixel in the frame buffer
   virtual void canvasPoint(STB::Colour colour, int32_t x, int32_t y) = 0;

   //! Get colour of single pixel in the frame buffer
   virtual STB::Colour canvasGetPixel(int32_t x, int32_t y) const { return HIDDEN; }

   //! Get pointer to underlying frame buffer
   virtual const PLT::Image* canvasGetImage() const { return nullptr; }

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

   //! Set a horizontal row of pixels in the frame buffer
   virtual void canvasSpan(STB::Colour colour, int32_t x1, int32_t y, int32_t x2)
   {
      assert(x1 < x2);

      for(int32_t x = x1; x < x2; x++)
      {
         canvasPoint(colour, x, y);
      }
   }

   //! Blit from another canvas into this canvas
   virtual void canvasBlit(const Canvas& source,
                           int32_t  x,     int32_t  y,
                           uint32_t w,     uint32_t h,
                           int32_t  src_x, int32_t  src_y)
   {
      assert(int32_t(src_x + w) <= source.getWidth());
      assert(int32_t(src_y + h) <= source.getHeight());

      for(uint32_t u = 0; u < w; u++)
      {
         for(uint32_t v = 0; v < h; v++)
         {
            canvasPoint(source.getPixel(src_x + u, src_y + v), x + u, y + v);
         }
      }
   }

private:
   //! Clip value to between 0 and limit (inclusive range)
   void clip(int32_t& value, int32_t limit)
   {
      if(value < 0)
         value = 0;
      else if(value > limit)
         value = limit;
   }

   //! Clip two values to between 0 and limit (inclusive range) and
   //  ensure swap the values if the first is greater than the second
   void clipAndSort(int32_t& value1, int32_t& value2, int32_t limit)
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
      assert(bpp <= LOG2_MAX_ALPHA_BPP);

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

   static const uint32_t LOG2_MAX_ALPHA_BPP = 4;

   Vector      size{0, 0};
   STB::Colour alpha_table[1 << LOG2_MAX_ALPHA_BPP] = {};
};

} // namespace GUI
