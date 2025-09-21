//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

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
   Layout() = default;

   //! Get position within top level rendering canvas
   const Vector& getPos() const { return pos; }

   //! Get size
   const Vector& getSize() const { return size; }

   //! Has horizontally placed children
   bool isRow() const { return row; }

   //! Has vertically placed children
   bool isCol() const { return not isRow(); }

   //! Is (x, y) in the widget
   bool isHit(signed x_, signed y_) const
   {
      return (x_ >= pos.x) && (x_ < (pos.x + size.x)) &&
             (y_ >= pos.y) && (y_ < (pos.y + size.y));
   }

   //! A horizontal container
   void setRow(bool row_ = true) { row = row_; }

   //! A vertical container
   void setCol(bool col_ = true) { setRow(not col_); }

   //! Set a fixed size for this item
   void setSize(unsigned width_, unsigned height_)
   {
      size.x = width_;
      size.y = height_;

      horz_fit = vert_fit = Fit::FIXED;
   }

   //! This item will shrink to fit it's children
   void setShrink() { horz_fit = vert_fit = Fit::SHRINK; }

   //! This item will expand to fit it's parent
   void setExpand() { horz_fit = vert_fit = Fit::EXPAND; }

   //! Translate children
   void nudgeChildren(signed x_, signed y_)
   {
      top_left.x += x_;
      top_left.y += y_;

      btm_right.x -= x_;
      btm_right.y -= y_;
   }

   //! Set border around and gap between children
   void setBorderAndGap(unsigned space_)
   {
      gap = top_left.x = top_left.y = btm_right.x = btm_right.y = space_;
   }

   //! Set alignment of children
   void setAlign(Align horz_align_, Align vert_align_)
   {
      horz_align = horz_align_;
      vert_align = vert_align_;
   }

protected:
   //! Set the width and height of the area occupied by children
   void setChildrenArea(const Vector& area_)
   {
      if (horz_fit != Fit::FIXED)
      {
         size.x = top_left.x + area_.x + btm_right.x;
      }

      if (vert_fit != Fit::FIXED)
      {
         size.y = top_left.y + area_.y + btm_right.y;
      }
   }

   void getChildrenArea(Vector& area_)
   {
      area_.x = size.x - top_left.x - btm_right.x;
      area_.y = size.y - top_left.y - btm_right.y;
   }

   Vector    pos{0, 0};               //!< Absolute position in root canvas
   Vector    size{0, 0};              //!< Width and height
   SmallSize top_left{0, 0};          //!< Top left border for children
   SmallSize btm_right{0, 0};         //!< Bottom right border for children
   bool      row{false};              //!< Layout children in a row not a column
   uint8_t   gap{0};                  //!< Gap between children
   Fit       horz_fit{Fit::FIXED};    //!< Horizontal fit
   Align     horz_align{Align::LEFT}; //!< Horizontal alignment mode for children
   Fit       vert_fit{Fit::FIXED};    //!< Vertical fit
   Align     vert_align{Align::TOP};  //!< Vertical alignment mode for children
};

} // namespace GUI
