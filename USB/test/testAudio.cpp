//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "USB/Audio/MIDIStreaming.h"

#include "STB/Test.h"

TEST(USB_Audio, structure_sizes)
{
// GCC is not happy with these structs at the moment
#if defined(__clang__)
   EXPECT_EQ(size_t(7),         sizeof(USB::MS::HeaderDescr) - sizeof(USB::Descr));
   EXPECT_EQ(size_t(6),         sizeof(USB::MS::JackInDescr) - sizeof(USB::Descr));
   EXPECT_EQ(size_t(7 + 2 * 2), sizeof(USB::MS::JackOutDescr<2>) - sizeof(USB::Descr));
   EXPECT_EQ(size_t(4 + 2),     sizeof(USB::MS::CSEndPointDescr<2>) - sizeof(USB::Descr));
   EXPECT_EQ(size_t(9),         sizeof(USB::MS::EndPointDescr) - sizeof(USB::Descr));
#endif
}
