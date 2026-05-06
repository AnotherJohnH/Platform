#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            Dma.cpp
            ../rp2xxx/Uart.cpp
            ../rp2xxx/MTL_console.cpp
            MTL_init.cpp
            MTL_start_core.cpp
            MTL_clock.cpp
            MTL_mutex.cpp
            ../core/CortexM0/MTL_excep.cpp
            Config.cpp)

target_include_directories(MTL PUBLIC include)
