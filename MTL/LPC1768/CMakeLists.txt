#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_init.cpp
            ../MTL_halt.cpp
            ../MTL_fault.cpp
            MTL_alert.cpp
            MTL_console.cpp
            ../core/CortexM3/MTL_clock.cpp
            ../core/CortexM3/MTL_excep.cpp)

target_include_directories(MTL PUBLIC include)
