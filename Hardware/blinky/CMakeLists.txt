#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_TARGET} STREQUAL rp2040)

   set(hw_blinky_config_list
       BADGER2040
       TUFTY2040
       RPIPICO
       CACHE INTERNAL "blinky config Variations")

elseif(${PLT_TARGET} STREQUAL rp2350)

   set(hw_blinky_config_list
       RPIPICO
       CACHE INTERNAL "blinky config Variations")

elseif(${PLT_TARGET} STREQUAL LPC11U24)

   set(hw_blinky_config_list
       MBED
       CACHE INTERNAL "blinky config Variations")

elseif(${PLT_TARGET} STREQUAL LPC1768)

   set(hw_blinky_config_list
       MBED
       CACHE INTERNAL "blinky config Variations")

elseif(${PLT_TARGET} STREQUAL nRF51)

   set(hw_blinky_config_list
       MICROBIT
       CACHE INTERNAL "blinky config Variations")

elseif(${PLT_TARGET} STREQUAL nRF52)

   set(hw_blinky_config_list
       MICROBIT
       CACHE INTERNAL "blinky config Variations")

elseif(${PLT_TARGET} STREQUAL native)

   set(hw_blinky_config_list
       NATIVE
       CACHE INTERNAL "blinky config Variations")

endif()

#-------------------------------------------------------------------------------
# Helper function for building binaries for each hardware config

function(add_blinky_executable target_name)

    foreach(hw_config ${hw_blinky_config_list})

        set(target ${target_name}_${hw_config})

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
        target_compile_definitions(${target} PRIVATE HW_${hw_config})
        target_link_libraries(     ${target} PRIVATE ${libs} HW)

        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    endforeach()

endfunction()

#-------------------------------------------------------------------------------
# Test

add_blinky_executable(blinkyEx   example.cpp)
