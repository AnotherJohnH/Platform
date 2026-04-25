#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TNY STATIC
            abort.cpp
            abs.cpp
            atof.cpp
            atoi.cpp
            atol.cpp
            atoll.cpp
            cos.c
            cosf.c
            clock.cpp
            delete.cpp
            exit.cpp
            expf.cpp
            fabsf.cpp
            fprintf.cpp
            feof.cpp
            fgetc.cpp
            fputc.cpp
            feof.cpp
            fstreams.cpp
            getchar.cpp
            global_obj.cpp
            guard.c
            logf.cpp
            malloc.cpp
            pow.c
            powf.cpp
            printf.cpp
            pure_virtual.cpp
            putchar.cpp
            puts.cpp
            rand.cpp
            sin.c
            sinf.c
            sqrt.c
            sqrtf.c
            scanf.cpp
            sleep.cpp
            srand.cpp
            strtol.cpp
            strtoll.cpp
            strtoul.cpp
            strtoull.cpp
            system.cpp
            tanhf.c
            time.cpp
            usleep.cpp
            vfprintf.cpp)

target_include_directories(TNY PUBLIC include)

target_link_libraries(TNY PUBLIC MTL)
