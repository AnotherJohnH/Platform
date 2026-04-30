#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_halt.cpp
            MTL_init.cpp
            MTL_console.cpp
            target/MTL/core/MTL_excep.cpp
            target/MTL/core/MTL_clock.cpp)

target_include_directories(MTL PUBLIC target)
