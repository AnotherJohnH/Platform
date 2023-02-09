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

add_library(TNY STATIC
            abort.cpp
            abs.cpp
            atof.cpp
            atoi.cpp
            atol.cpp
            atoll.cpp
            clock.cpp
            delete.cpp
            exit.cpp
            fprintf.cpp
            getchar.cpp
            global_obj.cpp
            guard.c
            isalpha.cpp
            isdigit.cpp
            isspace.cpp
            malloc.cpp
            memcpy.cpp
            memmove.cpp
            memset.cpp
            printf.cpp
            pure_virtual.cpp
            putchar.cpp
            puts.cpp
            rand.cpp
            sin.c
            scanf.cpp
            snprintf.cpp
            sprintf.cpp
            srand.cpp
            strcat.cpp
            strchr.cpp
            strcmp.cpp
            strcpy.cpp
            strlen.cpp
            strncmp.cpp
            strncpy.cpp
            strrchr.cpp
            strtol.cpp
            strtoll.cpp
            strtoul.cpp
            strtoull.cpp
            time.cpp
            tinyc_init.c
            vfprintf.cpp
            vsnprintf.cpp
            vsprintf.cpp)

target_include_directories(TNY PUBLIC include)
