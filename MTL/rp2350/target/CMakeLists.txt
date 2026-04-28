#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            Dma.cpp
            ../../rp2xxx/target/Uart.cpp
            ../../rp2xxx/target/MTL_console.cpp
            MTL_init.cpp
            MTL_start_core.cpp
            MTL_clock.cpp
            ../../core/CortexM33/MTL_excep.cpp
            Config.cpp)

target_include_directories(MTL PUBLIC .)
