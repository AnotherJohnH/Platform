//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_SWITCH_H
#define SND_SWITCH_H

#include "SND/SignalOp.h"

namespace SND {

//! Select one of multiple inputs
template <unsigned N>
class Switch : public NaryOp<N>, public Control::Observer
{
public:
   ControlIn<unsigned> control;

   Switch()
      : control(this)
   {
   }

private:
   virtual void controlEvent(Control*) override
   {
      control = control % N;
   }

   virtual Signal output() override
   {
      return this->in[control];
   }
};

} // namespace SND

#endif
