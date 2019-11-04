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

#include "GUI/Vector.h"

namespace GUI {

//! Size fit
enum class Fit : uint8_t
{
   FIXED = 0, // Size cannot change
   SHRINK,    // Shrink to minimum size that contains children
   EXPAND     // Expand to maximum size that parent allows
};

//! Alignment for children
enum class Align : uint8_t
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
public:
   //! Get position within top level rendering canvas
   const Vector& getPos()  const { return pos; }

   //! Get size
   const Vector& getSize() const { return size; }

   bool isRow() const { return row; }

   bool isHit(signed x_, signed y_) const
   {
      return (x_ >= pos.x) && (x_ < (pos.x + size.x)) &&
             (y_ >= pos.y) && (y_ < (pos.y + size.y));
   }

   void setRow(bool row_ = true)
   {
      row = row_;
   }

   //! Set a fixed size for this item
   void setSize(unsigned width, unsigned height)
   {
      size.x = width;
      size.y = height;

      horz_fit = vert_fit = Fit::FIXED;
   }

   //! This items will shrink to fit it's children
   void setShrink() { horz_fit = vert_fit = Fit::SHRINK; }

   //! This items will expand to fit it's parent
   void setExpand() { horz_fit = vert_fit = Fit::EXPAND; }

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
      gap = top_left.x = top_left.y = btm_right.x = btm_right.y = space;
   }

   //! Set alignment of children
   void setAlign(Align horz_align_, Align vert_align_)
   {
      horz_align = horz_align_;
      vert_align = vert_align_;
   }

protected:
   Vector      pos{0, 0};               //!< Absolute position in root canvas
   Vector      size{0, 0};              //!< Width and height
   Fit         horz_fit{Fit::FIXED};    //!< Horizontal fit
   Fit         vert_fit{Fit::FIXED};    //!< Vertical fit
   SmallSize   top_left{0, 0};          //!< Top left border for children
   SmallSize   btm_right{0, 0};         //!< Bottom right border for children
   bool        row{false};              //!< Layout children in a row not a column
   uint8_t     gap{0};                  //!< Gap between children
   Align       horz_align{Align::LEFT}; //!< Horizontal alignment mode for children
   Align       vert_align{Align::TOP};  //!< Vertical alignment mode for children

   //! Set the width and height of the area occupied by children
   void setChildrenArea(const Vector& area)
   {
      if(horz_fit != Fit::FIXED)
      {
         size.x = top_left.x + area.x + btm_right.x;
      }

      if(vert_fit != Fit::FIXED)
      {
         size.y = top_left.y + area.y + btm_right.y;
      }
   }

   void getChildrenArea(Vector& area)
   {
      area.x = size.x - top_left.x - btm_right.x;
      area.y = size.y - top_left.y - btm_right.y;
   }
};

} // namespace GUI

#endif
