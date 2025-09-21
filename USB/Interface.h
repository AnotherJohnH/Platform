//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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

   //! Called to handle device specific setup requests
   virtual bool handleSetupReqIn(uint8_t req_)
   {
      return false;
   }

   //! Called to handle device specific setup requests
   virtual bool handleSetupReqOut(uint8_t req_, uint8_t** ptr_, unsigned* bytes_)
   {
      return false;
   }

   //! Called when USB host sends a packet to this device
   virtual void buffRx(uint8_t ep_, const uint8_t* data_, unsigned len_) {}

   //! Called after a packet has been sent to the USB host from this device
   virtual void buffTx(uint8_t ep_) {}

   STB::List<Descr> descr_list{};

private:
   USB::InterfaceDescr descr;
};

} // namespace USB
