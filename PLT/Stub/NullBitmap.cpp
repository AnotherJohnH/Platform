//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Bitmap implementation that is broken!

#include "PLT/Bitmap.h"

namespace PLT {

Bitmap::Bitmap(unsigned width_, unsigned height_)
{
   resize(width_, height_);
}

Bitmap::~Bitmap()
{
}

void Bitmap::resize(unsigned width_, unsigned height_)
{
   width  = width_;
   height = height_;
   pitch  = (width * Image::getPixelBits() + 7) / 8;
   buffer = nullptr;
}

void* Bitmap::getHandle() const
{
   // No acceleration possible
   return nullptr;
}

} // namespace PLT
