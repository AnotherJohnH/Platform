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
# Build PLT library

add_library(PLT STATIC
            $<TARGET_OBJECTS:plt_obj>
            $<TARGET_OBJECTS:plt_obj_cpp_containers>
            $<TARGET_OBJECTS:plt_target_obj>)

target_compile_options(PLT PUBLIC ${PLT_C_FLAGS})

target_include_directories(PLT PUBLIC include)

get_target_property(target_compile_definitions plt_target_obj COMPILE_DEFINITIONS)
get_target_property(target_link_libraries      plt_target_obj LINK_LIBRARIES)
get_target_property(target_link_options        plt_target_obj LINK_OPTIONS)

target_compile_definitions(PLT PUBLIC ${target_compile_definitions})

target_link_libraries(PLT PUBLIC ${target_link_libraries})

target_link_options(PLT PUBLIC ${target_link_options})

#-------------------------------------------------------------------------------
# Testing

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

   add_subdirectory(googletest)

   add_subdirectory(Source/STB)
   add_subdirectory(Source/GUI)
   add_subdirectory(Source/SND)
   add_subdirectory(Source/TRM)
   add_subdirectory(Source/PLT/test)

   # TODO fix this build regression
   #add_subdirectory(Source/MTL)

endif()

#-------------------------------------------------------------------------------
# Installation

install(TARGETS PLT ARCHIVE DESTINATION lib)
