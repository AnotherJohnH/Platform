//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_CONTROL_OP_H
#define SND_CONTROL_OP_H

#include <cstdlib>

#include "SND/Control.h"

namespace SND {


template <typename TYPE>
class ControlMul : public ControlPort<TYPE>
{
public:
   ControlIn<TYPE> control;

   ControlMul(const TYPE& initial = 1.0)
      : control(this)
   {
      control = initial;
   }

private:
   virtual void controlEvent(Control*) override
   {
      this->send(this->recv() * control);
   }
};


template <typename TYPE>
class ControlAdd : public ControlPort<TYPE>
{
public:
   ControlIn<TYPE> control;

   ControlAdd(const TYPE& initial = 0.0)
      : control(this)
   {
      control = initial;
   }

private:
   virtual void controlEvent(Control*) override
   {
      this->send(this->recv() + control);
   }
};


} // namespace SND

#endif
