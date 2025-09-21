//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Bitmap implementation using a simple block of memory

#include "PLT/Bitmap.h"

namespace PLT {

Bitmap::Bitmap(unsigned width_, unsigned height_)
{
   resize(width_, height_);
}

Bitmap::~Bitmap()
{
   delete[] buffer;
}

void Bitmap::resize(unsigned width_, unsigned height_)
{
   delete[] buffer;

   width  = width_;
   height = height_;
   pitch  = (width * Image::getPixelBits() + 7) / 8;
   buffer = new uint8_t[pitch * height];
}

void* Bitmap::getHandle() const
{
   // No acceleration possible
   return nullptr;
}

} // namespace PLT
