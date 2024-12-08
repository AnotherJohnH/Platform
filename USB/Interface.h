//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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
// \brief Manage a USB Interface

#pragma once

#include "USB/Descr.h"

namespace USB {

class Interface : public STB::List<Interface>::Elem
{
public:
   using List = STB::List<Interface>;

   Interface(List&   list_,
             uint8_t class_,
             uint8_t sub_class_,
             uint8_t protocol_ = 0)
      : descr(descr_list, class_, sub_class_, protocol_)
   {
      list_.push_back(this);
   }

   //! Called after USB host has set the configuration for this device
   virtual void configured() {}

   //! Called when USB host sends a packet to this device
   virtual void buffRx(uint8_t ep_, const uint8_t* data_, unsigned len_) {}

   //! Called when after a packet has been sent to the USB host from this device
   virtual void buffTx(uint8_t ep_) {}

   STB::List<Descr> descr_list{};

private:
   USB::InterfaceDescr descr;
};

} // namespace USB
