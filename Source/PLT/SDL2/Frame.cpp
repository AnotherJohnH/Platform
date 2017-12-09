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

#include "SDL_headers.h"

#include "PLT/Frame.h"

#include <string>


static PLT::Frame::Scanner*  scanner{nullptr};


namespace PLT {


class Frame::Impl
{
public:
   Impl(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
      : title(title_)
      , flags(flags_)
   {
      static bool sdl_init = false;

      if(!sdl_init)
      {
         SDL_Init(SDL_INIT_VIDEO);
         sdl_init = true;
      }

      if((width_ != 0) && (height_ != 0))
      {
         createWindow(width_, height_);
      }
   }

   void createWindow(unsigned width_, unsigned height_)
   {
      Uint32 sdl_flags = 0;

#ifndef PROJ_TARGET_Emscripten
      sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
#endif

      if(flags & Frame::FULL_SCREEN) sdl_flags |= SDL_WINDOW_FULLSCREEN;
      if(flags & Frame::RESIZABLE)   sdl_flags |= SDL_WINDOW_RESIZABLE;
      if(flags & Frame::NO_BORDER)   sdl_flags |= SDL_WINDOW_BORDERLESS;

      window = SDL_CreateWindow(title.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width_, height_,
                                sdl_flags);

      renderer = SDL_CreateRenderer(window, -1, 0);

      SDL_RaiseWindow(window);

      createSurface(width_, height_);
   }

   ~Impl()
   {
      destroySurface();

#ifndef PROJ_TARGET_Emscripten
      SDL_DestroyRenderer(renderer);
#endif

      SDL_DestroyWindow(window);

#ifndef PROJ_TARGET_Emscripten
      // TODO is this the right thing to do? what if more than one Frame is in use?
      SDL_Quit();
#endif
   }

   uint8_t* getStorage(unsigned& pitch)
   {
      if(surface == nullptr) return nullptr;

      pitch = surface->pitch;
      return (uint8_t*)surface->pixels;
   }

   void* getHandle() const
   {
      return surface;
   }

   void resize(unsigned width_, unsigned height_)
   {
      if(window == nullptr)
      {
         if((width_ != 0) && (height_ != 0))
         {
            createWindow(width_, height_);
         }
         return;
      }

      destroySurface();
      SDL_SetWindowSize(window, width_, height_);
      createSurface(width_, height_);
   }

   void refresh()
   {
      SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
      SDL_RenderCopy(renderer, texture, nullptr, nullptr);
      SDL_RenderPresent(renderer);
   }

private:
   std::string   title;
   uint32_t      flags;
   SDL_Window*   window{nullptr};
   SDL_Renderer* renderer{nullptr};
   SDL_Surface*  surface{nullptr};
   SDL_Texture*  texture{nullptr};

   void createSurface(unsigned width_, unsigned height_)
   {
      surface = SDL_CreateRGBSurface(0, width_, height_, 32,
                                     0x00FF0000,
                                     0x0000FF00,
                                     0x000000FF,
                                     0xFF000000);

// TODO understand what's going on here!
#ifdef PROJ_TARGET_Emscripten
      texture = SDL_CreateTextureFromSurface(renderer, surface);
#else
      texture = SDL_CreateTexture(renderer,
                                  SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  width_, height_);
#endif
   }

   void destroySurface()
   {
      SDL_DestroyTexture(texture);
      SDL_FreeSurface(surface);
   }
};


Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   pimpl  = new Impl(title_, width, height, flags_);
   buffer = pimpl->getStorage(pitch);
}

Frame::~Frame() { delete pimpl; }

void* Frame::getHandle() const
{
   return pimpl->getHandle();
}

void Frame::resize(unsigned width_, unsigned height_)
{
   if((width == width_) && (height == height_)) return;

   width  = width_;
   height = height_;

   pimpl->resize(width_, height_);
   buffer = pimpl->getStorage(pitch);
}

void Frame::refresh()
{
   if (scanner != nullptr)
   {
      for(unsigned y=0; y<height; y++)
      {
         scanner->getRawPixels(buffer + y*pitch, y);
      }
   }

   pimpl->refresh();
}

void Frame::setScanner(Scanner* scanner_)
{
   scanner = scanner_;
}

} // namespace PLT
