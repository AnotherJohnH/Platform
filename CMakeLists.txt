#-------------------------------------------------------------------------------
#  Copyright (c) 2019 John D. Haughton
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

cmake_minimum_required(VERSION 3.11.4)

include(build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

project(Platform
        VERSION   0.0.5
        LANGUAGES C CXX ASM)

set(author  "John D. Haughton")

set(test_source Source/STB/testEndian.cpp
                Source/STB/testLicense.cpp)

#-------------------------------------------------------------------------------

install(TARGETS PLT ARCHIVE DESTINATION lib)

#-------------------------------------------------------------------------------

link_libraries(${PLT_libs})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(${CMAKE_SOURCE_DIR}/Source/MTL)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/PLT/test)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/GUI)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/SND)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/TRM)
add_subdirectory(${CMAKE_SOURCE_DIR}/Source/STB)
