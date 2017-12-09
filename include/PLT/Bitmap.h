//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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
//! \brief Platform abstraction interface for memory image buffer

#ifndef PLT_BITMAP_H
#define PLT_BITMAP_H

#include <stdint.h>

#include "Image.h"

//! Platform abstraction layer
namespace PLT {

//! Memory image buffer
class Bitmap : public Image
{
public:
   //! Construct a new bitmap
   //
   //! \param name
   //! \param width (pixels)
   //! \param height (pixels)
   Bitmap(const char* name, unsigned width, unsigned height);

   ~Bitmap();

   //! Get a platform specific handle for the bitmap
   virtual void* getHandle() const override;

private:
   class Impl;

   Impl* pimpl{nullptr};
};

} // namespace PLT

#endif
