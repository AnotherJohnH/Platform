#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_init.cpp
            ../core/CortexM0/MTL_clock.cpp
            ../core/CortexM0/MTL_excep.cpp)

target_include_directories(MTL PUBLIC include)
