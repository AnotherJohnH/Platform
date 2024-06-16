#-------------------------------------------------------------------------------
#  Copyright (c) 2023 John D. Haughton
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

add_subdirectory(${PLT_TARGET})

target_sources(MTL PRIVATE
               MTL_init.cpp
               MTL_load.cpp
               MTL_main.cpp
               MTL_halt.cpp
               MTL_clock.cpp
               MTL_putch.cpp
               MTL_getch.cpp
               MTL_PS2KeyDecode.cpp
               PALVideo.cpp)

# This warning seems broken in gcc 10 and later
target_compile_options(MTL PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Wno-stringop-overflow>")
target_compile_options(MTL PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Wno-stringop-overflow>")

target_compile_definitions(MTL PUBLIC MTL_TARGET=1)
target_compile_definitions(MTL PUBLIC MTL_${PLT_TARGET}=1)
target_compile_definitions(MTL PUBLIC MTL_VERSION="${PROJECT_VERSION}")
target_compile_definitions(MTL PUBLIC MTL_COMMIT="${PLT_COMMIT}")

target_include_directories(MTL PUBLIC ..)
