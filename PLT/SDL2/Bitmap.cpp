//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// SDL2 Bitmap implementation

#include "SDL_headers.h"

#include "PLT/Bitmap.h"

namespace PLT {

//! Memory image buffer
class Bitmap::Impl
{
public:
   Impl() = default;

   ~Impl()
   {
      resize(0, 0);
   }

   void resize(unsigned width_, unsigned height_)
   {
      if (surface != nullptr)
      {
         SDL_FreeSurface(surface);
      }

      if ((width_ == 0) || (height_ == 0))
      {
         return;
      }

#if 0
      surface = SDL_CreateRGBSurfaceWithFormat(0, width_, height_, 32, PIXEL_FORMAT);
#else
      // XXX replace with code above when SDL2.0.5 is more widely available
      surface = SDL_CreateRGBSurface(0, width_, height_, 32,
                                     0x00FF0000,
                                     0x0000FF00,
                                     0x000000FF,
                                     0xFF000000);
#endif
   }

   uint8_t* getData(unsigned& width_, unsigned& height_, unsigned& pitch_)
   {
      if(surface == nullptr)
      {
         return nullptr;
      }

      width_  = surface->w;
      height_ = surface->h;
      pitch_  = surface->pitch;

      return (uint8_t*)surface->pixels;
   }

   void* getHandle() const { return surface; }

private:
   SDL_Surface* surface{nullptr};
};


Bitmap::Bitmap(unsigned width_, unsigned height_)
   : Image(width_, height_)
{
   pimpl = new Impl();

   resize(width_, height_);
}

Bitmap::~Bitmap()
{
   delete pimpl;
}

void Bitmap::resize(unsigned width_, unsigned height_)
{
   pimpl->resize(width_, height_);

   buffer = pimpl->getData(width, height, pitch);
}

void* Bitmap::getHandle() const
{
   return pimpl->getHandle();
}

} // namespace PLT
