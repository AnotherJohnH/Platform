//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

// See "Universal Serial Bus Mass Storage Class - Bulk Only Transport"

#pragma once

#include <cstdint>

namespace SCSI {


struct CommandBlockWrapper
{
   static const unsigned LENGTH  = 31;
   static const uint8_t  FLAG_IN = 0b10000000;

   static const CommandBlockWrapper* validate(const uint8_t* raw_, unsigned length_)
   {
      const CommandBlockWrapper* cbw = (const CommandBlockWrapper*)raw_;

      if ((length_ != LENGTH) ||
          (cbw->signature[0] != 'U') ||
          (cbw->signature[1] != 'S') ||
          (cbw->signature[2] != 'B') ||
          (cbw->signature[3] != 'C'))
          return nullptr;

      return cbw;
   }

   bool isInput() const { return flags == FLAG_IN; }

   uint8_t  signature[4] = {'U', 'S', 'B', 'C'};
   uint32_t tag{};
   uint32_t transfer_length{};
   uint8_t  flags{};
   uint8_t  lun{};
   uint8_t  cmd_len{};
   uint8_t  cmd[16] = {};

} __attribute__((__packed__));


struct CommandStatusWrapper
{
   static const unsigned LENGTH             = 13;
   static const uint8_t  STATUS_OK          = 0x00;
   static const uint8_t  STATUS_FAILED      = 0x01;
   static const uint8_t  STATUS_PHASE_ERROR = 0x02;

   void setOk()     { status = STATUS_OK; }
   void setFailed() { status = STATUS_FAILED; }

   uint8_t  signature[4] = {'U', 'S', 'B', 'S'};
   uint32_t tag;
   uint32_t data_residue{0};
   uint8_t  status{STATUS_OK};

} __attribute__((__packed__));


enum Command : uint8_t
{
   TEST                         = 0x00,
   INQUIRY                      = 0x12,
   MODE_SENSE_6                 = 0x1A,
   START_STOP_UNIT              = 0x1B,
   PREVENT_ALLOW_MEDIUM_REMOVAL = 0x1E,
   READ_CAPACITY_10             = 0x25,
   READ_10                      = 0x28,
   WRITE_10                     = 0x2A,
   MODE_SENSE_10                = 0x5A
};


struct InquiryResponse
{
   uint8_t peripheral_type{0x00}; //!< Direct access block type
   uint8_t removable{0x80};       //!< Default to removable
   uint8_t version{0x00};         //!< No version claimed!
   uint8_t rdf{0x01};             //!< Value mandated by spec!
   uint8_t additional_length = sizeof(InquiryResponse) - 5;
   uint8_t flags1{};
   uint8_t flags2{};
   uint8_t flags3{};
   char    vendor_id[8];
   char    product_id[16];
   char    product_rev[4];

} __attribute__((__packed__));


struct Read10Command
{
   uint8_t  op_code{READ_10};
   uint8_t  flags;
   uint32_t lba;
   uint8_t  group;
   uint16_t len;
   uint8_t  control;

} __attribute__((__packed__));


struct Write10Command
{
   uint8_t  op_code{WRITE_10};
   uint8_t  flags;
   uint32_t lba;
   uint8_t  group;
   uint16_t len;
   uint8_t  control;

} __attribute__((__packed__));


} // namespace SCSI
