//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/Endian.h"

#include "STB/Test.h"

TEST(STB_Endian, Big16)
{
   STB::Big16     big16;
   const uint8_t* byte = (const uint8_t*)&big16;

   big16 = 0x1234;

   EXPECT_EQ(byte[0], 0x12);
   EXPECT_EQ(byte[1], 0x34);

   big16 = STB::endianSwap(big16);

   EXPECT_EQ(byte[0], 0x34);
   EXPECT_EQ(byte[1], 0x12);
}

TEST(STB_Endian, Big32)
{
   STB::Big32     big32;
   const uint8_t* byte = (const uint8_t*)&big32;

   big32 = 0x12345678;

   EXPECT_EQ(byte[0], 0x12);
   EXPECT_EQ(byte[1], 0x34);
   EXPECT_EQ(byte[2], 0x56);
   EXPECT_EQ(byte[3], 0x78);

   big32 = STB::endianSwap(big32);

   EXPECT_EQ(byte[0], 0x78);
   EXPECT_EQ(byte[1], 0x56);
   EXPECT_EQ(byte[2], 0x34);
   EXPECT_EQ(byte[3], 0x12);
}

TEST(STB_Endian, Big64)
{
   STB::Big64     big64;
   const uint8_t* byte = (const uint8_t*)&big64;

   big64 = 0x0123456789ABCDEF;

   EXPECT_EQ(byte[0], 0x01);
   EXPECT_EQ(byte[1], 0x23);
   EXPECT_EQ(byte[2], 0x45);
   EXPECT_EQ(byte[3], 0x67);
   EXPECT_EQ(byte[4], 0x89);
   EXPECT_EQ(byte[5], 0xAB);
   EXPECT_EQ(byte[6], 0xCD);
   EXPECT_EQ(byte[7], 0xEF);

   big64 = STB::endianSwap(big64);

   EXPECT_EQ(byte[0], 0xEF);
   EXPECT_EQ(byte[1], 0xCD);
   EXPECT_EQ(byte[2], 0xAB);
   EXPECT_EQ(byte[3], 0x89);
   EXPECT_EQ(byte[4], 0x67);
   EXPECT_EQ(byte[5], 0x45);
   EXPECT_EQ(byte[6], 0x23);
   EXPECT_EQ(byte[7], 0x01);
}

TEST(STB_Endian, Ltl16)
{
   STB::Ltl16     ltl16;
   const uint8_t* byte = (const uint8_t*)&ltl16;

   ltl16 = 0x1234;

   EXPECT_EQ(byte[0], 0x34);
   EXPECT_EQ(byte[1], 0x12);

   ltl16 = STB::endianSwap(ltl16);

   EXPECT_EQ(byte[0], 0x12);
   EXPECT_EQ(byte[1], 0x34);
}

TEST(STB_Endian, Ltl32)
{
   STB::Ltl32     ltl32;
   const uint8_t* byte = (const uint8_t*)&ltl32;

   ltl32 = 0x12345678;

   EXPECT_EQ(byte[0], 0x78);
   EXPECT_EQ(byte[1], 0x56);
   EXPECT_EQ(byte[2], 0x34);
   EXPECT_EQ(byte[3], 0x12);

   ltl32 = STB::endianSwap(ltl32);

   EXPECT_EQ(byte[0], 0x12);
   EXPECT_EQ(byte[1], 0x34);
   EXPECT_EQ(byte[2], 0x56);
   EXPECT_EQ(byte[3], 0x78);
}

TEST(STB_Endian, Ltl64)
{
   STB::Ltl64     ltl64;
   const uint8_t* byte = (const uint8_t*)&ltl64;

   ltl64 = 0x0123456789ABCDEF;

   EXPECT_EQ(byte[0], 0xEF);
   EXPECT_EQ(byte[1], 0xCD);
   EXPECT_EQ(byte[2], 0xAB);
   EXPECT_EQ(byte[3], 0x89);
   EXPECT_EQ(byte[4], 0x67);
   EXPECT_EQ(byte[5], 0x45);
   EXPECT_EQ(byte[6], 0x23);
   EXPECT_EQ(byte[7], 0x01);

   ltl64 = STB::endianSwap(ltl64);

   EXPECT_EQ(byte[0], 0x01);
   EXPECT_EQ(byte[1], 0x23);
   EXPECT_EQ(byte[2], 0x45);
   EXPECT_EQ(byte[3], 0x67);
   EXPECT_EQ(byte[4], 0x89);
   EXPECT_EQ(byte[5], 0xAB);
   EXPECT_EQ(byte[6], 0xCD);
   EXPECT_EQ(byte[7], 0xEF);
}
