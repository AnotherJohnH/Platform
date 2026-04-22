#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PDK_TARGET} STREQUAL rp2040)

   set(hwr_blinky_config_variants
       BADGER2040
       TUFTY2040
       RPIPICO
       CACHE INTERNAL "blinky configVariants")

elseif(${PDK_TARGET} STREQUAL rp2350)

   set(hwr_blinky_config_variants
       RPIPICO
       CACHE INTERNAL "blinky configVariants")

elseif(${PDK_TARGET} STREQUAL LPC11U24)

   set(hwr_blinky_config_variants
       MBED
       CACHE INTERNAL "blinky configVariants")

elseif(${PDK_TARGET} STREQUAL LPC1768)

   set(hwr_blinky_config_variants
       MBED
       CACHE INTERNAL "blinky configVariants")

elseif(${PDK_TARGET} STREQUAL nRF51)

   set(hwr_blinky_config_variants
       MICROBIT
       CACHE INTERNAL "blinky configVariants")

elseif(${PDK_TARGET} STREQUAL nRF52)

   set(hwr_blinky_config_variants
       MICROBIT
       CACHE INTERNAL "blinky configVariants")

elseif(${PDK_NATIVE})

   set(hwr_blinky_config_variants
       NATIVE
       CACHE INTERNAL "blinky configVariants")

endif()

#-------------------------------------------------------------------------------
# Helper function for building binaries for each hardware config

function(add_blinky_executable target_name)

    foreach(variant ${hwr_blinky_config_variants})

        set(target ${target_name}_${variant})

        set(sources "")
        set(libs "")
        set(mode "source")

        foreach(arg IN LISTS ARGN)
           if (arg STREQUAL "LIBRARIES")
              set(mode "libs")
           elseif(mode STREQUAL "source")
               list(APPEND sources ${arg})
           else()
               list(APPEND libs ${arg})
           endif()
        endforeach()

        add_executable(${target} ${sources})
        target_include_directories(${target} PUBLIC  ${CMAKE_CURRENT_BINARY_DIR})
        target_compile_definitions(${target} PRIVATE HW_${variant})
        target_link_libraries(     ${target} PRIVATE ${libs} HWR)

        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    endforeach()

endfunction()

#-------------------------------------------------------------------------------
# Test

if(BUILD_TESTING)
   add_blinky_executable(blinkyEx   example.cpp)
endif()
