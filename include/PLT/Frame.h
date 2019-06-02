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

//! \file Frame.h
//! \brief Platform abstraction interface for raw frame buffer

#ifndef PLT_FRAME_H
#define PLT_FRAME_H

#include <stdint.h>

#include "Image.h"

//! Platform abstraction layer
namespace PLT {

//! Raw frame buffer
class Frame : public Image
{
public:
   // Falgs for Frame creation
   static const uint32_t NO_FLAGS    = 0;
   static const uint32_t RESIZABLE   = 1 << 0; //!< Frame can be resized
   static const uint32_t NO_BORDER   = 1 << 1; //!< Frame has no external border pixels
   static const uint32_t FULL_SCREEN = 1 << 2; //!< Frame should cover the whole screen

   // Hints to request a logical pixel size larger than physical pixel size
   static uint32_t SCALE_X(unsigned n) { return ((n - 1) & 0xF) << 4; }
   static uint32_t SCALE_Y(unsigned n) { return ((n - 1) & 0xF) << 8; }

   class Scanner
   {
   public:
      virtual void getRawPixels(uint8_t* buffer, unsigned line) = 0;
   };

   //! Construct a new frame
   //
   //! \param title for window based platforms should be the window title
   //! \param width frame width (pixels)
   //! \param height frame height (pixels)
   //! \param flags bitmask of hints for the underlying platform that implements the frame
   Frame(const char* title, unsigned width_, unsigned height_, uint32_t flags_ = NO_FLAGS);

   ~Frame();

   //! Get a platform specific handle for the frame
   virtual void* getHandle() const override;

   //! Get a platform specific id for the frame
   uint32_t getId() const;

   //! Change the title, for window based platforms should be the window title
   void setTitle(const char* title);

   //! Hide or make visible a window
   void setVisible(bool visible);

   //! Resize the frame buffer
   //
   //! \param width frame width (pixels)
   //! \param height frame height (pixels)
   void resize(unsigned width_, unsigned height_);

   //! Ensure any changes to the frame buffer are displayed
   void refresh();

   //! Add a line scanning call-back
   void setScanner(Scanner* scanner_);

   //! Translate internal event co-ordinates to pixel co-ordinates
   //  For internal use by the PLT implementation when the Frame
   //  can support high-DPI (OS X) or pixel scaling
   static void internal_transEventXyToPixel(uint16_t& x, uint16_t& y);

private:
   class Impl;

   Impl*    pimpl{nullptr};
};

} // namespace PLT

#endif
