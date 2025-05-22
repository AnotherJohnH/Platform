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

#include "STB/FileSystem.h"
#include "STB/Endian.h"

#include "USB/Device.h"
#include "USB/Interface.h"
#include "USB/SCSI.h"

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
      for(unsigned i = 0; i < sizeof(TYPE); ++i)
      {
         if (*text_ == '\0')
            field[i] = ' ';
         else
            field[i] = *text_++;
      }
   }

   void scsiCommand(const uint8_t* bytes_, unsigned len_)
   {
      switch(bytes_[0])
      {
      case SCSI::TEST:
         printf("SCSI TEST\n");
         break;

      case SCSI::START_STOP_UNIT:
         printf("SCSI START STOP\n");
         break;

      case SCSI::INQUIRY:
         {
            printf("SCSI INQUIRY\n");

            SCSI::InquiryResponse response{};

            response.version = 2;
            setStr(response.vendor_id, "Platform");
            setStr(response.product_id, "Disk Image");
            setStr(response.product_rev, "0.0");

            bulk_out.write(&response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         break;

      case SCSI::MODE_SENSE_6:
         {
            printf("SCSI MODE SENSE 6\n");

            uint32_t response;

            response = 0x43;

            bulk_out.write(&response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         break;

      case SCSI::PREVENT_ALLOW_MEDIUM_REMOVAL:
         printf("SCSI PREVENT ALLOW MEDIUM REMOVAL\n");
         break;

      case SCSI::READ_CAPACITY_10:
         {
            printf("SCSI READ CAPACITY 10\n");

            uint32_t response[2];

            response[0] = STB::endianSwap(uint32_t(file_system->getNumBlocks()));
            response[1] = STB::endianSwap(uint32_t(file_system->getBlockSize()));

            bulk_out.write(&response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         break;

      case SCSI::MODE_SENSE_10:
         {
            printf("SCSI MODE SENSE 10\n");

            uint32_t response;

            response = STB::endianSwap(uint32_t(3));

            bulk_out.write(&response, sizeof(response));
            bulk_out.startTx(sizeof(response));
            to_send_count = 1;
         }
         break;

      case SCSI::READ_10:
         {
            auto cmd = reinterpret_cast<const SCSI::Read10Command*>(bytes_);

            lba           = STB::endianSwap(cmd->lba);
            segment       = 0;
            to_send_count = STB::endianSwap(cmd->len) * segments_per_block;

            printf("SCSI READ 10: ");
            printf("%03X+%u\n", lba, STB::endianSwap(cmd->len));

            bulk_out.write(file_system->get64BytePtr(lba, segment++), 64);
            bulk_out.startTx(64);
         }
         break;

      case SCSI::WRITE_10:
         {
            auto cmd = reinterpret_cast<const SCSI::Write10Command*>(bytes_);

            lba           = STB::endianSwap(cmd->lba);
            segment       = 0;
            to_recv_count = STB::endianSwap(cmd->len) * segments_per_block;

            printf("SCSI WRITE 10: ");
            printf("%03X-%u\n", lba, STB::endianSwap(cmd->len));
         }
         break;

      default:
         printf("SCSI command =");
         for(unsigned i = 0; i < len_; ++i)
         {
            printf(" %02x", bytes_[i]);
         }
         printf("\n");
         break;
      }
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
      bulk_in.startRx(64);
   }

   void buffRx(uint8_t ep_, const uint8_t* data_, unsigned length_) override
   {
      if (to_recv_count == 0)
      {
         const SCSI::CommandBlockWrapper* cbw = SCSI::CommandBlockWrapper::validate(data_, length_);

         if (cbw != nullptr)
         {
            csw.tag = cbw->tag;
            csw.setOk();

            scsiCommand(cbw->cmd, cbw->cmd_len);
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
         --to_recv_count;

         if (to_recv_count == 0)
         {
            bulk_out.write(&csw, csw.LENGTH);
            bulk_out.startTx(csw.LENGTH);
         }
         else
         {
            bulk_in.startRx(64);
         }
      }
   }

   void buffTx(uint8_t ep_) override
   {
      if (to_send_count > 0)
      {
         to_send_count--;

         if (to_send_count == 0)
         {
            bulk_out.write(&csw, csw.LENGTH);
            bulk_out.startTx(csw.LENGTH);
         }
         else
         {
            bulk_out.write(file_system->get64BytePtr(lba, segment++), 64);
            bulk_out.startTx(64);

            if (segment == segments_per_block)
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

   MTL::Usb::EndPoint bulk_in{d_bulk_in};
   MTL::Usb::EndPoint bulk_out{d_bulk_out};

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
