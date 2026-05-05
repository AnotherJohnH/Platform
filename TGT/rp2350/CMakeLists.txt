#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TGT STATIC
            boot.s
            vector_table_core0.s
            vector_table_core1.s)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m33 -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(TGT PUBLIC MTL ${gcc_lib})
