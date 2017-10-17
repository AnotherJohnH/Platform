//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

#ifndef PLT_FRAME_H
#define PLT_FRAME_H

#include <stdint.h>

#include "Image.h"

namespace PLT {

class FrameImpl;

class Frame : public Image
{
private:
   FrameImpl* pimpl{};

public:
   static const uint32_t RESIZABLE   = 1 << 0;
   static const uint32_t NO_BORDER   = 1 << 1;
   static const uint32_t FULL_SCREEN = 1 << 2;

   Frame(const char* title, unsigned width_, unsigned height_, uint32_t flags_ = 0);

   ~Frame();

   //!
   virtual void blit(unsigned x, unsigned y, unsigned src_offset, unsigned src_width,
                     const Image& src) override;

   //! Resize frame buffer
   void resize(unsigned width_, unsigned height_);

   //! Ensure changes in frame buffer are displayed
   void refresh();
};

} // namespace PLT

#endif
