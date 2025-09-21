#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_halt.cpp
            MTL/LPC1768/vector_table.s
            MTL/LPC1768/MTL_init.cpp
            MTL/LPC1768/MTL_console.cpp
            MTL/CortexM3/MTL_excep.cpp
            MTL/CortexM3/MTL_clock.cpp)

target_include_directories(MTL PUBLIC .)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m3 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(MTL PUBLIC ${gcc_lib})
