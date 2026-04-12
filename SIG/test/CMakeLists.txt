#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   foreach(variant IN LISTS SIG_VARIANTS)

       set(lib  SIG_${variant})
       set(test testSIG_${variant})

       add_executable(${test}
                      testMain.cpp
                      testConv.cpp
                      testDelay.cpp
                      testDelayN.cpp
                      testDelayV.cpp
                      testExpSlew.cpp
                      testGain.cpp
                      testLinSlew.cpp
                      testLogPot.cpp
                      testOsc.cpp
                      testEnv.cpp)

       target_link_libraries(${test} ${lib} STB)

       add_test(NAME ${test} COMMAND ${test} )

   endforeach()

endif()
