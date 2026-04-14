//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/FixP.h"

#include "STB/Test.h"

TEST(STB_FixP, iostream)
{
   STB::FixP<16> value_16{1.99};
   std::cout << value_16 << std::endl;

   STB::FixP<0> value_0{value_16};
   std::cout << value_0 << std::endl;

   STB::FixP<1> value_1{value_16};
   std::cout << value_1 << std::endl;

   STB::FixP<2> value_2{value_16};
   std::cout << value_2 << std::endl;

   STB::FixP<3> value_3{value_16};
   std::cout << value_3 << std::endl;

   STB::FixP<4> value_4{value_16};
   std::cout << value_4 << std::endl;

   STB::FixP<5> value_5{value_16};
   std::cout << value_5 << std::endl;

   STB::FixP<31> value_32{value_16};
   std::cout << value_32 << std::endl;
}

TEST(STB_FixP, basic)
{
   STB::FixP<16> fx_value{1.89};
   double fl_value = fx_value;
   EXPECT_NEAR(1.89, fl_value, 0.01);

   fx_value += 1.0;
   fl_value = fx_value;
   EXPECT_NEAR(2.89, fl_value, 0.01);

   fx_value *= 2.0;
   fl_value = fx_value;
   EXPECT_NEAR(5.78, fl_value, 0.01);

   fx_value /= 2.0;
   fl_value = fx_value;
   EXPECT_NEAR(2.89, fl_value, 0.01);

   fx_value -= 1.0;
   fl_value = fx_value;
   EXPECT_NEAR(1.89, fl_value, 0.01);
}
