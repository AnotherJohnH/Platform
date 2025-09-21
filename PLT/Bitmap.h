//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
   //! Create a blank bitmap
   //
   //! \param width (pixels)
   //! \param height (pixels)
   Bitmap(unsigned width = 0, unsigned height = 0);

   ~Bitmap();

   //! Resize the bitmap, the current pixel data is lost
   void resize(unsigned width, unsigned height);

   //! Get a platform specific handle for the bitmap
   virtual void* getHandle() const override;

private:
   class Impl;

   Impl* pimpl{nullptr};
};

} // namespace PLT

#endif
