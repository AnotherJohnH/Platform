//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

// SDL2 Bitmap implementation

#include <cstdio>

#include "SDL2/SDL.h"

#include "PLT/Bitmap.h"

namespace PLT {

//! Memory image buffer
class Bitmap::Impl : public Image
{
public:
   Impl(unsigned width, unsigned height)
   {
      surface = SDL_CreateRGBSurface(0, width, height, 32,
                                     0x00FF0000,
                                     0x0000FF00,
                                     0x000000FF,
                                     0xFF000000);
   }

   Impl(const char* filename)
   {
      surface = SDL_LoadBMP(filename);
      if (surface == nullptr)
      {
         fprintf(stderr, "Failed to open file '%s'\n", filename);
      }
   }

   ~Impl()
   {
      SDL_FreeSurface(surface);
   }

   void getSize(unsigned& width, unsigned& height)
   {
      if (surface != nullptr)
      {
         width  = surface->w;
         height = surface->h;
      }
      else
      {
         width  = 0;
         height = 0;
      }
   }

   uint8_t* getStorage(unsigned& pitch)
   {
      if(surface == nullptr) return nullptr;

      pitch = surface->pitch;
      return (uint8_t*)surface->pixels;
   }

   virtual void* getHandle() const override
   {
      return surface;
   }

   void save(const char* name)
   {
      char filename[FILENAME_MAX];

      sprintf(filename, "%s.bmp", name);

      SDL_SaveBMP(surface, filename);
   }

private:
   SDL_Surface*   surface{nullptr};
};


Bitmap::Bitmap(unsigned width_, unsigned height_)
   : Image(width_, height_)
{
   pimpl = new Impl(width_, height_);

   buffer = pimpl->getStorage(pitch);
}

Bitmap::Bitmap(const char* filename_)
{
   pimpl  = new Impl(filename_);

   pimpl->getSize(width, height);
   buffer = pimpl->getStorage(pitch);
}

Bitmap::~Bitmap()
{
   delete pimpl;
}

void* Bitmap::getHandle() const
{
   return pimpl->getHandle();
}

void Bitmap::save(const char* name) const
{
   pimpl->save(name);
}

} // namespace PLT
