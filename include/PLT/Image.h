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
   uint32_t getPixel(unsigned x, unsigned y) const;

   //! Clear to the given colour
   void clear(uint32_t rgb);

   //! Write a pixel in the image
   //
   // \param rgb 24-bit colour
   // \param x X co-ordinate (pixels)
   // \param y Y co-ordinate (pixels)
   void point(uint32_t rgb, unsigned x, unsigned y);

   //! Draw a horizontal line of pixels
   //
   // \param rgb 24-bit colour
   // \param x1 Start X co-ordinate (pixels)
   // \param y Y co-ordinate (pixels)
   // \param x2 End X co-ordinate (pixels)
   void span(uint32_t rgb, unsigned x1, unsigned y, unsigned x2);

   //! Blit another image into this image
   //
   // \param x X co-ordinate in target image (pixels)
   // \param y Y co-ordinate in target image (pixels)
   // \param src_offset x offset into source (pixels)
   // \param src_width length of each line from source to render (pixels)
   // \param source Reference to source image
   void blit(unsigned x, unsigned y, unsigned src_offset, unsigned src_width,
             const Image& source);

protected:
   virtual ~Image() {}

   //! Clear entire image (back-stop slow implementation)
   void defaultClear(uint32_t rgb)
   {
      for(unsigned y = 0; y < height; y++)
      {
         span(rgb, 0, y, width);
      }
   }

   //! Draw a horizontal line of pixels (back-stop slow implementation)
   void defaultSpan(uint32_t rgb, unsigned x1, unsigned y, unsigned x2)
   {
      for(unsigned x = x1; x < x2; x++)
      {
         point(rgb, x, y);
      }
   }

   //! Blit another image into this image (back-stop slow implementation)
   void defaultBlit(unsigned x, unsigned y, unsigned src_offset, unsigned src_width,
                    const Image& source)
   {
      assert((src_offset + src_width) <= source.getWidth());

      for(unsigned u = 0; u < src_width; u++)
      {
         for(unsigned v = 0; v < source.getHeight(); v++)
         {
            point(source.getPixel(src_offset + u, v), x + u, y + v);
         }
      }
   }

   uint8_t* buffer{nullptr};
   unsigned pitch{0};
   unsigned width{0};
   unsigned height{0};
};

} // namespace PLT

#endif
