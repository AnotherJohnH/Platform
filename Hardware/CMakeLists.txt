#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

foreach(hw ${HARDWARE})
   add_subdirectory(${hw})
endforeach()

#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

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

   target_compile_definitions(HW PUBLIC PLT_BCD_VERSION=${plt_bcd_version})

endif()

