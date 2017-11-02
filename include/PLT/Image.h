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

   //! Write a pixel in the image
   //
   // \param x X co-ordinate (pixels)
   // \param y Y co-ordinate (pixels)
   void setPixel(unsigned x, unsigned y, uint32_t rgb);

   //! Blit another image into this image
   //
   // \param x X co-ordinate in target image (pixels)
   // \param y Y co-ordinate in target image (pixels)
   // \param src_offset x offset into source (pixels)
   // \param src_width length of each line from source to render (pixels)
   // \param source Reference to source image
   virtual void blit(unsigned x, unsigned y, unsigned src_offset, unsigned src_width,
                     const Image& source)
   {
      // Back-stop slow implementation
      assert((src_offset + src_width) <= source.getWidth());

      for(unsigned u = 0; u < src_width; u++)
      {
         for(unsigned v = 0; v < source.getHeight(); v++)
         {
            setPixel(x + u, y + v, source.getPixel(src_offset + u, v));
         }
      }
   }

protected:
   uint8_t* buffer{nullptr};
   unsigned pitch{0};
   unsigned width{0};
   unsigned height{0};

   virtual ~Image() {}
};

} // namespace PLT

#endif
