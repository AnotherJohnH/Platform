//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief USB File Device

#pragma once

#if defined(HW_USB_FILE_DEVICE)

#include "MTL/USBMassStorageInterface.h"

#elif defined(HW_USB_FILE_NATIVE)

#include "STB/FileSystem.h"

#endif

namespace hw {

#if defined(HW_USB_FILE_DEVICE)

//! USB device with storage interface
class UsbFile : public MTL::USBDevice
{
public:
   UsbFile(uint16_t         device_id_,
           const char*      device_name_,
           STB::FileSystem& file_system_)
      : MTL::USBDevice("https://github.com/AnotherJohnH",
                       device_id_, PLT_BCD_VERSION, device_name_,
                       PLT_COMMIT)
      , storage_if{this, file_system_}
   {}

   MTL::USBMassStorageInterface storage_if;
};

#elif defined(HW_USB_FILE_NATIVE)

//! USB device with storage interface
class UsbFile
{
public:
   UsbFile(uint16_t         device_id_,
           const char*      device_name_,
           STB::FileSystem& file_system_)
   {
   }

   void irq() {}
};

#endif

} // namespace hw
