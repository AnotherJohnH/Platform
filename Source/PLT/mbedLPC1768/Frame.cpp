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

#include "../../MTL/chip/LPC1768/PALVideo.h"
#include "MTL/chip/LPC1768/RAM.h"

#include <cstring>


static MTL::PALVideo  video;
PAL_VIDEO_ATTACH_IRQ(video);

static PLT::Frame::Scanner* scanner{nullptr};

static void scanCallBack(uint8_t* buffer, uint16_t line)
{
   scanner->getRawPixels(buffer, line);
}


namespace PLT {

Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   MTL::RAM  ram;

   pimpl  = nullptr;
   buffer = ram.data();

   if (width_ != 0)
   {
      resize(width_, height_);
   }
}

Frame::~Frame()
{
   // nohing to do here
}

void* Frame::getHandle() const
{
   return nullptr;
}

void Frame::resize(unsigned width_, unsigned height_)
{
   width  = width_;
   height = height_;
   pitch  = ((width_ + 0x1F) & ~0x1F) / 8;

   video.setHorzPos(0);
   video.resize(pitch * 8, height);
   video.setFramePtr(buffer);

   memset(buffer, 0, height * pitch);
}

void Frame::refresh()
{
   // nothing to do here
}

void Frame::setScanner(Scanner* scanner_)
{
   scanner = scanner_;

   video.setScanner(scanner != nullptr ? scanCallBack : nullptr);
}

} // namespace PLT
