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

#pragma once

#include "MTL/chip/Usb.h"

#include "USB/Device.h"
#include "USB/Interface.h"

namespace MTL {

//! Standard Mass Stroarge interface
class MassStorageInterface : public USB::Interface
{
public:
   MassStorageInterface(List& list_)
      : USB::Interface(list_,
                       USB::CLASS_MASS_STORAGE,
                       USB::MS::SUB_CLASS_SCSI,
                       USB::MS::PROTOCOL_BULK_ONLY_TRANSPORT)
   {
   }

   USB::EndPointDescr bulk_in{ descr_list, USB::EndPointDescr::IN,  0x02};
   USB::EndPointDescr bulk_out{descr_list, USB::EndPointDescr::OUT, 0x02};
};


//! USB Mass Storage device
class USBMassStorageDevice : public USB::Device
{
public:
   USBMassStorageDevice(const char* vendor_name_,
                        uint16_t    product_id_,
                        uint16_t    bcd_version_,
                        const char* product_name_,
                        const char* serial_number_)
   {
      setVendor(vendor_name_);
      setProduct(product_id_, bcd_version_, product_name_);
      setSerialNumber(serial_number_);
   }

private:
   USB::Config          config{*this};
   MassStorageInterface ms_interface{config.interface_list};
};

}
