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

#ifndef PLT_FRAME_BASE_H
#define PLT_FRAME_BASE_H

#include <cassert>
#include <stdint.h>

namespace PLT {

//! Raw frame buffer
class FrameBase
{
protected:
   uint8_t*    buffer;
   unsigned    pitch;
   unsigned    width;
   unsigned    height;

   virtual ~FrameBase() {}

public:
   FrameBase(unsigned width_ = 0, unsigned height_ = 0)
      : buffer(0)
      , pitch(0)
      , width(width_)
      , height(height_)
   {}

   //! Returns the number of bits per pixel
   static unsigned getPixelBits();

   virtual void* getHandle() const { return 0; }

   //! Get width (pixels)
   unsigned getWidth() const { return width; }

   //! Get height (pixels)
   unsigned getHeight() const { return height; }

   //! Get pointer to memory mapped frame buffer
   uint8_t* getPointer(unsigned& pitch_) const
   {
      pitch_ = pitch;
      return buffer;
   }

   //! Get pixel
   uint32_t getPixel(unsigned x, unsigned y) const;

   //! Set pixel
   void setPixel(unsigned x, unsigned y, uint32_t rgb);

   //! 
   virtual void blit(unsigned x, unsigned y,
                     unsigned src_offset, unsigned src_width,
                     const FrameBase& source)
   {
       // Back-stop slow implementation
       // TODO this is broken
       for(unsigned i=0; i<source.getWidth(); i++)
       {
          for(unsigned j=0; j<source.getHeight(); j++)
          {
             setPixel(x+i, y+j, source.getPixel(i, j));
          }
       }
   }
};

} // namespace PLT

#endif
