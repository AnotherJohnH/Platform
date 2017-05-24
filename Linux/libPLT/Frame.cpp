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

#include "SDL.h"

#include "PLT/Frame.h"

namespace PLT {

class FrameImpl
{
private:
   SDL_Window*    window;
   SDL_Renderer*  renderer;
   SDL_Surface*   surface;
   SDL_Texture*   texture;

   static bool init;

   void createTexture(unsigned width_, unsigned height_)
   {
      texture = SDL_CreateTexture(renderer,
                                  SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  width_, height_);

      surface = SDL_CreateRGBSurface(0, width_, height_, 32,
                                     0x00FF0000,
                                     0x0000FF00,
                                     0x000000FF,
                                     0xFF000000);
   }

   void destroyTexture()
   {
      SDL_FreeSurface(surface);
      SDL_DestroyTexture(texture);
   }

public:
   FrameImpl(const char*  title_, unsigned width_, unsigned height_, uint32_t flags_)
   {
      if (!init)
      {
         SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

         init = true;
      }

      Uint32  sdl_flags = 0;

#ifndef PROJ_TARGET_Emscripten
     sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;
#endif

      if (flags_ & Frame::FULL_SCREEN)  sdl_flags |= SDL_WINDOW_FULLSCREEN;
      if (flags_ & Frame::RESIZABLE)    sdl_flags |= SDL_WINDOW_RESIZABLE;
      if (flags_ & Frame::NO_BORDER)    sdl_flags |= SDL_WINDOW_BORDERLESS;

      window = SDL_CreateWindow(title_,
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width_, height_,
                                sdl_flags);

      renderer = SDL_CreateRenderer(window, -1, 0);

      createTexture(width_, height_);

      SDL_RaiseWindow(window);
   }

   ~FrameImpl()
   {
      destroyTexture();

      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);

      SDL_Quit();
   }

   uint8_t* getPointer(unsigned& pitch)
   {
      pitch = surface->pitch;
      return (uint8_t*)surface->pixels;
   }

   void blit(unsigned     x,
             unsigned     y,
             unsigned     src_offset,
             unsigned     src_width,
             SDL_Surface* src)
   {
      SDL_Rect srcrect;
      SDL_Rect dstrect;

      srcrect.x = src_offset;
      srcrect.y = 0;
      srcrect.w = src_width;
      srcrect.h = src->h;

      dstrect.x = x;
      dstrect.y = y;
      dstrect.w = src_width;
      dstrect.h = src->h;

      if (src_width == 0)
      {
         srcrect.w = src->w;
         dstrect.w = src->w;
      }

      SDL_BlitSurface(src, &srcrect, surface, &dstrect);
   }

   void resize(unsigned width_, unsigned height_)
   {
      destroyTexture();
      SDL_SetWindowSize(window, width_, height_);
      createTexture(width_, height_);
   }

   void refresh()
   {
      SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, NULL, NULL);
      SDL_RenderPresent(renderer);
   }
};


bool FrameImpl::init = false;


Frame::Frame(const char* title_,
             unsigned    width_,
             unsigned    height_,
             uint32_t    flags_)
   : FrameBase(width_, height_)
{
   pimpl  = new FrameImpl(title_, width, height, flags_);
   buffer = pimpl->getPointer(pitch);
}

Frame::~Frame()
{
   delete pimpl;
}

void Frame::blit(unsigned x,
                 unsigned y,
                 unsigned src_offset,
                 unsigned src_width,
                 const FrameBase& src)
{
   pimpl->blit(x, y, src_offset, src_width, (SDL_Surface*)src.getHandle());
}

void Frame::resize(unsigned width_, unsigned height_)
{
   if ((width == width_) && (height == height_)) return;

   width  = width_;
   height = height_;

   pimpl->resize(width_, height_);
   buffer = pimpl->getPointer(pitch);
}

void Frame::refresh()
{
   pimpl->refresh();
}

} // PLT

