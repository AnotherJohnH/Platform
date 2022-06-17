#-------------------------------------------------------------------------------
#  Copyright (c) 2022 John D. Haughton
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

project(PLT
        LANGUAGES C CXX ASM
        VERSION 0.0.6)

include(config.cmake)

add_subdirectory(Source)

#-------------------------------------------------------------------------------
# Build

add_library(PLT STATIC
            $<TARGET_OBJECTS:plt_obj>
            $<TARGET_OBJECTS:plt_obj_cpp_containers>
            $<TARGET_OBJECTS:plt_target_obj>)

target_compile_options(PLT PUBLIC ${PLT_C_FLAGS})

target_link_libraries(PLT PUBLIC plt_target_obj)

#-------------------------------------------------------------------------------
# Unit testing

include(GoogleTest)

set(BUILD_GMOCK   OFF)
set(INSTALL_GTEST OFF)

add_subdirectory(googletest)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

   add_subdirectory(Source/STB)
   add_subdirectory(Source/GUI)
   add_subdirectory(Source/SND)
   add_subdirectory(Source/TRM)
   add_subdirectory(Source/PLT/test)
   add_subdirectory(Source/MTL)

#-------------------------------------------------------------------------------
# Installation

   install(TARGETS PLT ARCHIVE DESTINATION lib)
   install(DIRECTORY include DESTINATION include)

#-------------------------------------------------------------------------------
# Packaging

   set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
   set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
   include(CPack)

endif()
