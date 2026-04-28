#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PDK_TARGET} STREQUAL rp2040)

   set(hwr_picoSense_config_variants
       RPIPICO_WAVESHARE_EPAPER
       BADGER2040
       TUFTY2040
       CACHE INTERNAL "picoSense configVariants")

elseif(${PDK_TARGET} STREQUAL rp2350)

   set(hwr_picoSense_config_variants
       RPIPICO_WAVESHARE_EPAPER
       CACHE INTERNAL "picoSense configVariants")

elseif(${PDK_NATIVE})

   set(hwr_picoSense_config_variants
       NATIVE
       CACHE INTERNAL "picoSense configVariants")

endif()

#-------------------------------------------------------------------------------
# Helper function for building binaries for each hardware config

function(add_picoSense_executable target_name)

    foreach(variant ${hwr_picoSense_config_variants})

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
        target_link_libraries(     ${target} PRIVATE ${libs} GUI HWR)

        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    endforeach()

endfunction()

#-------------------------------------------------------------------------------
# Test

if(BUILD_TESTING)
   add_picoSense_executable(picoSenseTest test.cpp)
   add_picoSense_executable(picoSenseEx   example.cpp)
endif()
