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

// \brief mbed LPC1768 Frame implementation

#include "PLT/Frame.h"

#include "MTL/chip/LPC1768/PALVideo.h"
#include "MTL/chip/LPC1768/RAM.h"

#include <cstring>


#define WIDTH   512
#define HEIGHT  256


static MTL::PALVideo<WIDTH,HEIGHT>  video;
PAL_VIDEO_ATTACH_IRQ(video);


namespace PLT {

Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   MTL::RAM  ram;

   pimpl = nullptr;
   buffer = ram.data();
   pitch  = WIDTH / 8;

   memset(buffer, 0, ram.size());

   video.setFramePtr(buffer);
   video.setHorzPos(0);
}

Frame::~Frame()
{
   // nohing to do here
}

void* Frame::getHandle() const
{
   return nullptr;
}

void Frame::blit(unsigned x,
                 unsigned y,
                 unsigned src_offset,
                 unsigned src_width,
                 const Image& src)
{
   Image::blit(x, y, src_offset, src_width, src);
}

void Frame::resize(unsigned width_, unsigned height_)
{
   assert((width_ <= WIDTH) && (height <= HEIGHT_));
}

void Frame::refresh()
{
   // nohing to do here
}

} // namespace PLT
