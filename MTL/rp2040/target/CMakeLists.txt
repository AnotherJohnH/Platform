#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            boot2.s
            vector_table_core0.s
            vector_table_core1.s
            Dma.cpp
            ../../rp2xxx/target/Uart.cpp
            ../../rp2xxx/target/MTL_console.cpp
            MTL_init.cpp
            MTL_start_core.cpp
            MTL_clock.cpp
            MTL_mutex.cpp
            ../../core/CortexM0/MTL_excep.cpp
            Config.cpp)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m0 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_include_directories(MTL PUBLIC .)

target_link_libraries(MTL PUBLIC ${gcc_lib})
