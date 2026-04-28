#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(DEFINED HWR_CONFIG)

   message("HWR_CONFIG         = ${HWR_CONFIG}")
   message("--------------------------------------------------------------------------------")

   foreach(config ${HWR_CONFIG})
      add_subdirectory(${config})
   endforeach()

endif()

#-------------------------------------------------------------------------------

if(${PDK_NATIVE})

   add_library(HWR INTERFACE)

   target_link_libraries(HWR INTERFACE GUI PLT STB)

   target_include_directories(HWR INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

else()

   add_library(HWR STATIC
               fault_handler.cpp)

   target_link_libraries(HWR PUBLIC TNY TGT)

   target_include_directories(HWR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

   math(EXPR bcd_version "${CMAKE_MAJOR_VERSION} * 0x100 + ${CMAKE_MINOR_VERSION}"
        OUTPUT_FORMAT HEXADECIMAL)

   target_compile_definitions(HWR PUBLIC PDK_BCD_VERSION=${bcd_version})

endif()
