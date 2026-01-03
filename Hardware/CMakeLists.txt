#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Top level for stand-alone build

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

   cmake_minimum_required(VERSION 3.20)

   project(Hardware
           LANGUAGES CXX C ASM
           VERSION 0.01)

   include(CTest)

   set(HARDWARE blinky picoX7 picoX21H picoSense picoGame)

else()

   set(CMAKE_CXX_STANDARD          20   PARENT_SCOPE)
   set(CMAKE_CXX_STANDARD_REQUIRED True PARENT_SCOPE)

endif()

#-------------------------------------------------------------------------------

add_subdirectory(Platform)

if(${PLT_TARGET} STREQUAL rp2040 OR ${PLT_TARGET} STREQUAL rp2350)
    math(EXPR plt_bcd_version "${CMAKE_MAJOR_VERSION} * 0x100 + ${CMAKE_MINOR_VERSION}"
           OUTPUT_FORMAT HEXADECIMAL)
    target_compile_definitions(MTL PUBLIC PLT_BCD_VERSION=${plt_bcd_version})
endif()

foreach(hw ${HARDWARE})
   add_subdirectory(${hw})
endforeach()

#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL native)

   foreach(hw ${HARDWARE})
      set(hw_${hw}_config_list NATIVE CACHE INTERNAL "Config Variations")
   endforeach()

   add_library(HW INTERFACE)

   target_link_libraries(HW INTERFACE GUI PLT STB)

   target_include_directories(HW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

else()

   add_library(HW STATIC
               fault_handler.cpp)

   target_link_libraries(HW PUBLIC TNY MTL)

   target_include_directories(HW PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

   math(EXPR plt_bcd_version "${CMAKE_MAJOR_VERSION} * 0x100 + ${CMAKE_MINOR_VERSION}"
        OUTPUT_FORMAT HEXADECIMAL)

   target_compile_definitions(MTL PUBLIC PLT_BCD_VERSION=${plt_bcd_version})

endif()
