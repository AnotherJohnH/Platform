#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_clock.cpp
            MTL_init.cpp)

target_include_directories(MTL PUBLIC include)
