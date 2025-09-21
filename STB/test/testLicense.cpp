//-------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/License.h"

#include "STB/Test.h"

TEST(STB_License, MIT)
{
   EXPECT_EQ(strlen(MIT_LICENSE), 1022);
}
