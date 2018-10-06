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

#include <cstdio>

#include "SDL_headers.h"

#include "PLT/Image.h"

namespace PLT {

unsigned Image::getPixelBits() { return 32; }

uint32_t Image::getPixel(unsigned x, unsigned y) const
{
   uint32_t* pixels = reinterpret_cast<uint32_t*>(buffer);
   return pixels[x + y * pitch / 4];
}

void Image::clear(STB::Colour rgb)
{
   defaultClear(rgb);
}

void Image::point(STB::Colour rgb, unsigned x, unsigned y)
{
   uint32_t* pixels = reinterpret_cast<uint32_t*>(buffer);
#ifdef PROJ_TARGET_Emscripten
   pixels[x + y * pitch / 4] = 0xFF000000 | rgb;
#else
   pixels[x + y * pitch / 4] = rgb;
#endif
}

void Image::span(uint32_t rgb, unsigned x1, unsigned y, unsigned x2)
{
   defaultSpan(rgb, x1, y, x2);
}

void Image::blit(const Image& source,
                 unsigned x, unsigned y,
                 unsigned w, unsigned h,
                 unsigned src_x, unsigned src_y)
{
   SDL_Surface* src = static_cast<SDL_Surface*>(source.getHandle());

   SDL_Rect srcrect;
   srcrect.x = src_x;
   srcrect.y = src_y;
   srcrect.w = w;
   srcrect.h = h;

   SDL_Rect dstrect;
   dstrect.x = x;
   dstrect.y = y;
   dstrect.w = srcrect.w;
   dstrect.h = srcrect.h;

   SDL_BlitSurface(src, &srcrect, static_cast<SDL_Surface*>(getHandle()), &dstrect);
}

void Image::lineBlit(uint8_t pixel_mask, STB::Colour one, STB::Colour zero,
                     unsigned x, unsigned y)
{
   defaultLineBlit(pixel_mask, one, zero, x, y);
}

bool Image::save(const char* name) const
{
   return defaultSave(name);

   // .bmp is not the most portable
#if 0
   std::string filename = name;
   filename += ".bmp";
   SDL_SaveBMP(static_cast<SDL_Surface*>(getHandle()), filename.c_str());
#endif
}

} // namespace PLT
