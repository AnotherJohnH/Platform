//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief USB types

#pragma once

namespace USB {

enum class Request : uint8_t
{
   GET_STATUS     = 0x00,
   CLR_FEATURE    = 0x01,
   SET_FEATURE    = 0x03,
   SET_ADDRESS    = 0x05,
   GET_DESCRIPTOR = 0x06,
   SET_DESCRIPTOR = 0x07,
   GET_CONFIG     = 0x08,
   SET_CONFIG     = 0x09,
   GET_INTERFACE  = 0x0A,
   SET_INTERFACE  = 0x0B,
   SYNC_FRAME     = 0x0C
};

//! SETUP packet
struct SetupReq
{
   SetupReq() = default;

   unsigned isHostToDevice() const { return (type & 0b10000000) == 0; }
   unsigned getType()        const { return (type >> 5) & 0b11; }
   unsigned getRecipient()   const { return type & 0b11111; }

   uint8_t  type{};
   Request  request{};
   uint16_t value{};
   uint16_t index{};
   uint16_t length{};

} __attribute__((__packed__));


} // namespace USB
