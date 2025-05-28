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

#include "MTL/chip/USBDevice.h"

#include "STB/FileSystem.h"
#include "STB/Endian.h"

#include "USB/Device.h"
#include "USB/Interface.h"
#include "USB/SCSI.h"

#undef  LOG
#define LOG  if (0) printf

namespace MTL {

//! Standard Mass Stroarge interface
class USBMassStorageInterface : public USB::Interface
{
public:
   USBMassStorageInterface(USB::Device* device_, STB::FileSystem& file_system_)
      : USB::Interface(device_->getInterfaceList(),
                       USB::CLASS_MASS_STORAGE,
                       USB::MS::SUB_CLASS_SCSI,
                       USB::MS::PROTOCOL_BULK_ONLY_TRANSPORT)
   {
      file_system        = &file_system_;
      segments_per_block = file_system->getBlockSize() / 64;
   }

private:
   template <typename TYPE>
   void setStr(TYPE& field, const char* text_)
   {
      for(unsigned i = 0; i < sizeof(field); ++i)
      {
         if (*text_ == '\0')
            field[i] = ' ';
         else
            field[i] = *text_++;
      }
   }

   bool scsiCommand(const uint8_t* bytes_, unsigned len_)
   {
      switch(bytes_[0])
      {
      case SCSI::TEST:
         LOG("TEST\n");
         return true; // nop and acknowledge

      case SCSI::START_STOP_UNIT:
         LOG("START STOP LoEj=%u Start=%u\n", (bytes_[4] >> 1) & 1, bytes_[4] & 1);
         return true; // nop and acknowledge

      case SCSI::PREVENT_ALLOW_MEDIUM_REMOVAL:
         LOG("PREVENT ALLOW MEDIUM REMOVAL prevent=%u\n", bytes_[4] & 1);
         return true; // nop and acknowledge

      case SCSI::INQUIRY:
         {
            LOG("INQUIRY\n");

            SCSI::InquiryResponse response{};

            response.version = 2;
            setStr(response.vendor_id, "Platform");
            setStr(response.product_id, "Disk Image");
            setStr(response.product_rev, "0.0");

            bulk_out.write(&response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         return true;

      case SCSI::MODE_SENSE_6:
         {
            LOG("MODE SENSE 6\n");

            uint8_t response[4] = {0x03,  // mode data length bytes
                                   0x00,  // medium type
                                   0x00,  // device specific (b7 WriteProt)
                                   0x00}; // block descriptor length

            bulk_out.write(response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         return true;

      case SCSI::MODE_SENSE_10:
         {
            LOG("MODE SENSE 10\n");

            uint8_t response[8] = {0x00, 0x06,              // mode data length bytes
                                   0x00,                    // medium type
                                   0x00,                    // device specific (b7 WriteProt)
                                   0x00, 0x00, 0x00, 0x00}; // block descriptor length

            bulk_out.write(response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         return true;

      case SCSI::READ_CAPACITY_10:
         {
            LOG("READ CAPACITY 10\n");

            uint32_t response[2];

            response[0] = STB::endianSwap(uint32_t(file_system->getNumBlocks() - 1));
            response[1] = STB::endianSwap(uint32_t(file_system->getBlockSize()));

            bulk_out.write(&response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         return true;

      case SCSI::READ_10:
         {
            auto cmd = reinterpret_cast<const SCSI::Read10Command*>(bytes_);

            lba             = STB::safeReadBig32(&cmd->lba);
            unsigned blocks = STB::safeReadBig16(&cmd->len);

            LOG("READ 10: %03X+%u\n", lba, blocks);

            file_system->read(lba, /* offset */ 0, 64, bulk_out.writeBuffer());
            bulk_out.startTx(64);

            to_send_count = blocks * segments_per_block;
            segment       = 1;
         }
         return true;

      case SCSI::WRITE_10:
         {
            auto cmd = reinterpret_cast<const SCSI::Write10Command*>(bytes_);

            lba             = STB::safeReadBig32(&cmd->lba);
            unsigned blocks = STB::safeReadBig16(&cmd->len);

            LOG("WRITE 10: %03X-%u\n", lba, blocks);

            to_recv_count = blocks * segments_per_block;
            segment       = 0;
         }
         return true;

      default:
         LOG("SCSI command =");
         for(unsigned i = 0; i < len_; ++i)
         {
            LOG(" %02x", bytes_[i]);
         }
         LOG("\n");
         break;
      }

      return false;
   }

   bool handleSetupReqOut(uint8_t req_, uint8_t** ptr_, unsigned* bytes_) override
   {
      switch(req_)
      {
      case 0xFE: // Get Max LUN
         *ptr_   = &max_lun;
         *bytes_ = 1;
         return true;
      }

      return false;
   }

   void configured() override
   {
      // Ready to recieve first SCSI command
      bulk_in.startRx(64);
   }

   //! Incomming packet event
   void buffRx(uint8_t ep_, const uint8_t* data_, unsigned length_) override
   {
      if (to_recv_count == 0)
      {
         const SCSI::CommandBlockWrapper* cbw = SCSI::CommandBlockWrapper::validate(data_, length_);
         if (cbw != nullptr)
         {
            csw.tag = cbw->tag;

            if (scsiCommand(cbw->cmd, cbw->cmd_len))
               csw.setOk();
            else
               csw.setFailed();
         }
         else
         {
            csw.setFailed();
         }

         if (to_recv_count != 0)
         {
            bulk_in.startRx(64);
         }
         else if (to_send_count == 0)
         {
            bulk_out.write(&csw, csw.LENGTH);
            bulk_out.startTx(csw.LENGTH);
         }
      }
      else
      {
         file_system->write(lba, segment * 64, 64, data_);

         if (++segment == segments_per_block)
         {
            segment = 0;
            lba++;
         }

         if (--to_recv_count == 0)
         {
            bulk_out.write(&csw, csw.LENGTH);
            bulk_out.startTx(csw.LENGTH);

            file_system->endOfWrite();
         }
         else
         {
            bulk_in.startRx(64);
         }
      }
   }

   //! Outgoing packet just sent event
   void buffTx(uint8_t ep_) override
   {
      if (to_send_count > 0)
      {
         to_send_count--;

         if (to_send_count == 0)
         {
            bulk_out.write(&csw, csw.LENGTH);
            bulk_out.startTx(csw.LENGTH);

            file_system->endOfRead();
         }
         else
         {
            file_system->read(lba, segment * 64, 64, bulk_out.writeBuffer());
            bulk_out.startTx(64);

            if (++segment == segments_per_block)
            {
               segment = 0;
               lba++;
            }
         }
      }
      else
      {
         bulk_in.startRx(64);
      }
   }

   USB::EndPointDescr d_bulk_in{ descr_list, USB::EndPointDescr::OUT, USB::EndPointDescr::BULK};
   USB::EndPointDescr d_bulk_out{descr_list, USB::EndPointDescr::IN,  USB::EndPointDescr::BULK};

   MTL::USBEndPoint bulk_in{d_bulk_in};
   MTL::USBEndPoint bulk_out{d_bulk_out};

   unsigned                   segments_per_block{};
   unsigned                   to_send_count{0};
   unsigned                   to_recv_count{0};
   uint32_t                   lba{0};
   unsigned                   segment{0};
   SCSI::CommandStatusWrapper csw{};
   uint8_t                    max_lun{0};   //!< Just one
   STB::FileSystem*           file_system{nullptr};
};

} // namespace MTL
