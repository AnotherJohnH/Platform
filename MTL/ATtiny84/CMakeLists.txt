#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_init.cpp
            MTL_clock.cpp
            MTL_console.cpp)

target_include_directories(MTL PUBLIC include)
