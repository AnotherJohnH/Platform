//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief 

#include "MTL/chip/LPC1768/PALVideo.h"


#include "Frame.h"


#define WIDTH   320
#define HEIGHT  256


MTL::PALVideo<WIDTH,HEIGHT>  video("Test PALVideo", 2);
PAL_VIDEO_ATTACH_IRQ(video);


Frame1BPP<WIDTH,HEIGHT,MTL::PAL_VIDEO_BYTE_SWAP>   frame;


int main()
{
   video.setFramePtr(frame.getPtr());

   video.setHorzPos(0);

   frame.clear(true);
   frame.rect(1, 1, WIDTH-1, HEIGHT-1, false);

   frame.triangle(100, 10, 160, 80, 110, 120, true);

   frame.line(0,        0, WIDTH, HEIGHT, true);
   frame.line(0, HEIGHT-1, WIDTH,      0, true);

   for(unsigned x=0; x<32; x++)
   {
      for(unsigned y=0; y<32; y++)
      {
         frame.point(x + 32, y + 32, (x + y) & 1);
      }
   }

   video.refresh();
}
