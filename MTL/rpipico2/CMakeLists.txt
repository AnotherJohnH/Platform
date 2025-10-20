#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL/chip/boot.s
            MTL/chip/vector_table_core0.s
            MTL/chip/vector_table_core1.s
            MTL/chip/fault_handler.cpp
            MTL/chip/Dma.cpp
            MTL/chip/Uart.cpp
            MTL/chip/MTL_init.cpp
            MTL/chip/MTL_start_core.cpp
            MTL/chip/MTL_console.cpp
            MTL/chip/MTL_clock.cpp
            MTL/core/MTL_excep.cpp
            Config.cpp)

target_include_directories(MTL PUBLIC .)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m33 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(MTL PUBLIC ${gcc_lib})
