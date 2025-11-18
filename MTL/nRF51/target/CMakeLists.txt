#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            vector_table.s
            Leds.cpp
            MTL_halt.cpp
            MTL_init.cpp
            MTL/core/MTL_clock.cpp
            MTL/core/MTL_excep.cpp)

target_include_directories(MTL PUBLIC .)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m0 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_include_directories(MTL PUBLIC .)

target_link_libraries(MTL PUBLIC ${gcc_lib})
