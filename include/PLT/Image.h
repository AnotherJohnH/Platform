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

#ifndef PLT_IMAGE_H
#define PLT_IMAGE_H

#include <cassert>
#include <stdint.h>

namespace PLT {

//! A raw image that may have a platform specific implementation
class Image
{
protected:
   uint8_t* buffer{nullptr};
   unsigned pitch{0};
   unsigned width{0};
   unsigned height{0};

   virtual ~Image() {}

public:
   Image(unsigned width_ = 0, unsigned height_ = 0)
      : width(width_)
      , height(height_)
   {}

   //! Returns the number of bits per pixel
   static unsigned getPixelBits();

   //! Get frame width (pixels)
   unsigned getWidth() const { return width; }

   //! Get frame height (pixels)
   unsigned getHeight() const { return height; }

   //! Get a platform specific handle
   virtual void* getHandle() const { return 0; }

   //! Get pointer to the storage for the image
   uint8_t* getStorage(unsigned& pitch_) const
   {
      pitch_ = pitch;
      return buffer;
   }

   //! Read a pixel
   uint32_t getPixel(unsigned x, unsigned y) const;

   //! Write a pixel
   void setPixel(unsigned x, unsigned y, uint32_t rgb);

   //! Blit another image into this image
   virtual void blit(unsigned x, unsigned y, unsigned src_offset, unsigned src_width,
                     const Image& source)
   {
      // Back-stop slow implementation
      // TODO this is broken
      for(unsigned i = 0; i < source.getWidth(); i++)
      {
         for(unsigned j = 0; j < source.getHeight(); j++)
         {
            setPixel(x + i, y + j, source.getPixel(i, j));
         }
      }
   }
};

} // namespace PLT

#endif
