/*------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//----------------------------------------------------------------------------*/

//! \brief tiny C library implementation

#pragma once

#include <stdlib.h>
#include <stdarg.h>

#define  NULL   0
#define  FILENAME_MAX 256

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   long  pos;

} FILE;

extern FILE*   stdin;
extern FILE*   stdout;
extern FILE*   stderr;


enum { SEEK_SET, SEEK_CUR, SEEK_END };


extern FILE*   fopen(const char* filename, const char* mode);
extern size_t  fseek(FILE* stream, long offset, int whence);
extern long    ftell(FILE* stream);
extern size_t  fread(void*, size_t, size_t, FILE* stream);
extern size_t  fwrite(const void*, size_t, size_t, FILE* stream);
extern int     fclose(FILE* stream);

extern int     fgetc(FILE*);
extern int     fputc(int, FILE*);
extern int     fputs(const char*, FILE*);
extern int     fprintf(FILE*, const char*, ...);
extern int     vfprintf(FILE*, const char*, va_list);

extern int     puts(const char* s);
extern int     putchar(int c);
extern int     getchar();
extern char*   gets(char*);
extern int     printf(const char* format, ...);

extern int     snprintf(char*, size_t, const char*, ...);
extern int     sprintf(char*, const char*, ...);
extern int     vsnprintf(char*, size_t, const char*, va_list);
extern int     vsprintf(char*, const char*, va_list);


#ifdef __cplusplus
}
#endif
