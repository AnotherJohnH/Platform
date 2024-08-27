#-------------------------------------------------------------------------------
#  Copyright (c) 2024 John D. Haughton
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL/chip/boot.s
            MTL/chip/vector_table_core0.s
            MTL/chip/vector_table_core1.s
            MTL/chip/Uart.cpp
            MTL/chip/MTL_init.cpp
            MTL/chip/MTL_clock.cpp
            MTL/chip/MTL_console.cpp
            MTL/core/MTL_excep.cpp)

target_include_directories(MTL PUBLIC .)

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m33 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

target_link_libraries(MTL PUBLIC ${gcc_lib})
