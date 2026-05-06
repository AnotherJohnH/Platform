//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief

#pragma once

#include <stdint.h>
#include <cstdlib>
#include <cassert>
#include <utility>

namespace MTL {

template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned BPP,
          typename PIXEL,
          unsigned BYTE_SWAP>
class Frame
{
private:
   static const PIXEL BLACK = 0;

   static bool testX(int x) { return (x >= 0) && (x < int(WIDTH));  }

   static bool testY(int y) { return (y >= 0) && (y < int(HEIGHT)); }

   static bool clipRange(int& c1, int& c2, int LIMIT)
   {
      assert(c1 <= c2);

      if ((c1 >= LIMIT) || (c2 <= 0)) return false;

      if (c1 < 0)     c1 = 0;
      if (c2 > LIMIT) c2 = LIMIT;

      return true;
   }


   uint8_t frame[(WIDTH*BPP/8) * HEIGHT];


   void rawPoint(int x, int y, PIXEL colour)
   {
      assert(testX(x) && testY(y));

      unsigned index = ((x + y*WIDTH)*BPP/8) ^ BYTE_SWAP;
      uint8_t& byte  = frame[index];

      switch(BPP)
      {
      case  1:
         {
            uint8_t  mask  = 0x80>>(x%8);

            if (colour) byte |= mask;
            else        byte &= ~mask;
         }
         break;

      case  2:
         {
            switch(x & 3)
            {
            case 0: byte = (byte & 0xC0) | (colour<<6); break;
            case 1: byte = (byte & 0x30) | (colour<<4); break;
            case 2: byte = (byte & 0x0C) | (colour<<2); break;
            case 3: byte = (byte & 0x03) | colour;      break;
            }
         }
         break;

      case  4:
         {
            if (x & 1)  byte = (byte & 0xF0) | colour;
            else        byte = (byte & 0x0F) | (colour << 4);
         }
         break;

      case  8:
         byte = colour;
         break;

      default: break;
      }
   }


   void rawSpan(int x1, int x2, int y, PIXEL colour)
   {
      assert(x1 <= x2);

      for(int x=x1; x<x2; ++x)
      {
         this->rawPoint(x, y, colour);
      }
   }

   void rawRect(int x1, int y1, int x2, int y2, PIXEL colour)
   {
      assert(y1 <= y2);

      for(int y=y1; y<y2; ++y)
      {
         rawSpan(x1, x2, y, colour);
      }
   }

public:
   static unsigned getWidth()  { return WIDTH;  }

   static unsigned getHeight() { return HEIGHT; }

   //! Get pointer to frame buffer
   uint8_t* getPtr()
   {
      return frame;
   }

   //! Get pointer to frame buffer
   uint8_t* getPtr(unsigned x, unsigned y)
   {
      unsigned index = ((x + y*WIDTH)*BPP/8) ^ BYTE_SWAP;
      return frame + index;
   }

   //! Draw a point
   void point(int x, int y, PIXEL colour)
   {
      if (testX(x) && testY(y))
      {
         this->rawPoint(x, y, colour);
      }
   }

   //! Draw a span
   void span(int x1, int x2, int y, PIXEL colour)
   {
      if (testY(y) && clipRange(x1, x2, WIDTH))
      {
         rawSpan(x1, x2, y, colour);
      }
   }

   //! Filled rectangle
   void rect(int x1, int y1, int x2, int y2, PIXEL colour)
   {
      if (clipRange(x1, x2, WIDTH) && clipRange(y1, y2, HEIGHT))
      {
         rawRect(x1, y1, x2, y2, colour);
      }
   }

   //! Clear entire frame
   void clear(PIXEL colour = BLACK)
   {
      rawRect(0, 0, WIDTH, HEIGHT, colour);
   }

   //! Draw a straight line between two points
   void line(int x1, int y1, int x2, int y2, PIXEL colour)
   {
      int dx = x2 - x1;
      int dy = y2 - y1;

      if (abs(dx) >= abs(dy))
      {
         int ix = dx < 0 ? -1 : +1;
         int iy = (dy<<16)/abs(dx);
         int y  = y1<<16;

         for(int x=x1; x != x2; x += ix)
         {
            point(x, y>>16, colour);
            y += iy;
         }
      }
      else
      {
         int iy = dy < 0 ? -1 : +1;
         int ix = (dx<<16)/abs(dy);
         int x  = x1<<16;

         for(int y=y1; y != y2; y += iy)
         {
            point(x>>16, y, colour);
            x += ix;
         }
      }
   }

   //! Draw a filled triangle
   void triangle(int x1, int y1,
                 int x2, int y2,
                 int x3, int y3,
                 PIXEL colour)
   {
      if (y2 < y1)
      {
         std::swap(y2, y1);
         std::swap(x2, x1);
      }

      if (y3 < y1)
      {
         std::swap(y3, y1);
         std::swap(x3, x1);
      }

      if (y3 < y2)
      {
         std::swap(y3, y2);
         std::swap(x3, x2);
      }

      if (y3 == y1) return;
      
      int dxe=((x3-x1)<<16)/(y3 - y1);
      int dxs;

      int y=y1;
      int xs=x1<<16;
      int xe;

      if (y2 > y1)
      {
         xe=x1<<16;

         dxs=((x2-x1)<<16)/(y2 - y1);

         for(; y<y2; ++y)
         {
            if (xs < xe)
               span(xs>>16, xe>>16, y, colour);
            else
               span(xe>>16, xs>>16, y, colour);

            xs += dxs;
            xe += dxe;
         }
      }
      else
      {
         xe = x2<<16;
      }

      if (y3 == y2) return;

      dxs=((x3-x2)<<16)/(y3 - y2);

      for(; y<y3; ++y)
      {
         if (xs < xe)
            span(xs>>16, xe>>16, y, colour);
         else
            span(xe>>16, xs>>16, y, colour);

         xs += dxs;
         xe += dxe;
      }
   }

   void lineBlit(int x, int y, unsigned n, uint32_t pixels, PIXEL fg, PIXEL bg)
   {
      assert(n <= 32);

      for(uint32_t mask=1<<(n-1); mask; mask = mask >> 1)
      {
         rawPoint(x++, y, pixels & mask ? fg : bg);
      }
   }
};


template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned BYTE_SWAP=0>
class Frame1BPP : public Frame<WIDTH,HEIGHT,1,bool,BYTE_SWAP>
{
};


template <unsigned WIDTH,
          unsigned HEIGHT,
          unsigned BYTE_SWAP=0>
class Frame4BPP : public Frame<WIDTH,HEIGHT,4,uint8_t,BYTE_SWAP>
{
};

} // namespace MTL
