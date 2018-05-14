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

//! \file Image.h
//! \brief Platform specific bitmap image representation

#ifndef PLT_IMAGE_H
#define PLT_IMAGE_H

#include <cstdint>
#include <cassert>

#include "STB/Colour.h"

//! Platform abstraction layer
namespace PLT {

//! Base class for rectangular bitmap image
//
// Internal representation will be platform specific
class Image
{
public:
   Image(unsigned width_ = 0, unsigned height_ = 0)
      : width(width_)
      , height(height_)
   {}

   //! Returns the size of a pixel (bits)
   static unsigned getPixelBits();

   //! Get image width (pixels)
   unsigned getWidth() const { return width; }

   //! Get image height (pixels)
   unsigned getHeight() const { return height; }

   //! Get a platform specific handle for the image
   virtual void* getHandle() const { return nullptr; }

   //! Get pointer to the storage for the image
   //
   // \param pitch_ Length of each line of the image (bytes)
   uint8_t* getStorage(unsigned& pitch_) const
   {
      pitch_ = pitch;
      return buffer;
   }

   //! Read a pixel from the image
   //
   // \param x X co-ordinate (pixels)
   // \param y Y co-ordinate (pixels)
   STB::Colour getPixel(unsigned x, unsigned y) const;

   //! Clear to the given colour
   void clear(STB::Colour rgb);

   //! Write a pixel in the image
   //
   // \param rgb 24-bit colour
   // \param x X co-ordinate (pixels)
   // \param y Y co-ordinate (pixels)
   void point(STB::Colour rgb, unsigned x, unsigned y);

   //! Draw a horizontal line of pixels
   //
   // \param rgb 24-bit colour
   // \param x1 Start X co-ordinate (pixels)
   // \param y Y co-ordinate (pixels)
   // \param x2 End X co-ordinate (pixels)
   void span(STB::Colour rgb, unsigned x1, unsigned y, unsigned x2);

   //! Blit another image into this image
   //
   // \param x X co-ordinate in target image (pixels)
   // \param y Y co-ordinate in target image (pixels)
   // \param w width in target image (pixels)
   // \param h height in target image (pixels)
   // \param src_x X co-ordinate in source image (pixels)
   // \param src_y Y co-ordinate in source image (pixels)
   // \param source Reference to source image
   void blit(const Image& source,
             unsigned x, unsigned y, 
             unsigned w, unsigned h,
             unsigned src_x, unsigned src_y);

   //! Blit an 8-bit pixel mask
   void lineBlit(uint8_t pixel_mask, STB::Colour one, STB::Colour zero,
                 unsigned x, unsigned y);

   //! Save and to the named file (an appropriate extension will be added)
   void save(const char* name) const;

protected:
   virtual ~Image() {}

   //! Clear entire image (back-stop slow implementation)
   void defaultClear(STB::Colour rgb)
   {
      for(unsigned y = 0; y < height; y++)
      {
         span(rgb, 0, y, width);
      }
   }

   //! Draw a horizontal line of pixels (back-stop slow implementation)
   void defaultSpan(STB::Colour rgb, unsigned x1, unsigned y, unsigned x2)
   {
      for(unsigned x = x1; x < x2; x++)
      {
         point(rgb, x, y);
      }
   }

   //! Blit another image into this image (back-stop slow implementation)
   void defaultBlit(const Image& source,
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
            point(source.getPixel(src_x + u, src_y + v), x + u, y + v);
         }
      }
   }

   //! Blit an 8-bit pixel mask
   void defaultLineBlit(uint8_t pixels, STB::Colour one, STB::Colour zero,
                        unsigned x, unsigned y)
   {
      for(unsigned i=0; i<8; i++)
      {
         point(pixels & (1<<7) ? one : zero, x + i, y);
         pixels = pixels<<1;
      }
   }

   uint8_t* buffer{nullptr};
   unsigned pitch{0};
   unsigned width{0};
   unsigned height{0};
};

} // namespace PLT

#endif
