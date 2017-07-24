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

#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include <cstdint>
#include <vector>

#include "GUI/Paper.h"

namespace GUI {

// Size fit
enum Fit
{
   FIX = 0,
   SHRINK,    // Shrink to minimum size that contains children
   EXPAND     // Expand to maximum size that parent allows
};


// Alignment for children
enum Align
{
   LEFT = 0,
   CENTER,
   RIGHT,

   TOP    = LEFT,
   BOTTOM = RIGHT
};


//! Widget layout
class Layout
{
protected:
   Vector     pos;               //!< Absolute position in root paper
   Vector     size;              //!< Width and height
   Fit        horz_fit{FIX};     //!< Horizontal fit
   Fit        vert_fit{FIX};     //!< Vertical fit

   bool       row{false};        //!< Layout children in a row not a column
   Vector     top_left;          //!< Top left border for children
   Vector     btm_right;         //!< Bottom right border for children
   unsigned   gap{0};            //!< Gap between children
   Align      horz_align{LEFT};  //!< Horizontal alignment mode for children
   Align      vert_align{TOP};   //!< Vertical alignment mode for children

   Layout()
      : pos(0, 0)
      , size(0, 0)
      , top_left(0, 0)
      , btm_right(0, 0)
   {}

public:
   unsigned getX() const { return pos.x;  }
   unsigned getY() const { return pos.y;  }

   unsigned getWidth()  const { return size.x; }
   unsigned getHeight() const { return size.y; }

   bool isHit(unsigned x_, unsigned y_) const
   {
      return (x_ >= pos.x) && (x_ < (pos.x + size.x)) &&
             (y_ >= pos.y) && (y_ < (pos.y + size.y));
   }

   bool isRow() const { return row; }

   //! Set a fixed size for this item
   void setSize(unsigned width, unsigned height)
   {
      size.x = width;
      size.y = height;

      horz_fit = vert_fit = FIX;
   }

   //! This items will shrink to fit it's children
   void setShrink()
   {
      horz_fit = vert_fit = SHRINK;
   }

   //! This items will expand to fit it's parent 
   void setExpand()
   {
      horz_fit = vert_fit = EXPAND;
   }

   //! Translate children
   void nudgeChildren(signed x, signed y)
   {
      top_left.x += x;
      top_left.y += y;

      btm_right.x -= x;
      btm_right.y -= y;
   }

   //! Set border around and gap between children
   void setBorderAndGap(unsigned space)
   {
      top_left.x = top_left.y = btm_right.x = btm_right.y = space;
      gap = space;
   }

   //! Set alignment of children
   void setAlign(Align horz_align_, Align vert_align_)
   {
      horz_align = horz_align_;
      vert_align = vert_align_;
   }
};

} // namespace GUI

#endif
