//------------------------------------------------------------------------------
// Copyright (c) 2015-2017 John D. Haughton
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
//------------------------------------------------------------------------------

#ifndef STB_CONSOLE_APP_H
#define STB_CONSOLE_APP_H

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#include "STB/Option.h"
#include "STB/License.h"

namespace STB {

//! A generic class to simplify the creation of user friendly console applications
class ConsoleApp
{
private:
    Option<bool>   opt_version{'v', "version", "Display version information"};
    Option<bool>   opt_help{   'h', "help",    "Display this help"};
    Option<bool>   opt_debug{  'd', "debug",   "Report extra debug information"};

    static const char* extractFilename(const char* path)
    {
       // Windows is not supported
       const char* filename = strrchr(path, '/');

       return filename != nullptr ? filename + 1
                                  : path;
    }

    static void normal() { printf("\033[0m"); }
    static void bold()   { printf("\033[1m"); }

    void showVersion()
    {
        printf("\n");
        printf("Program     : %s\n",    program);
        printf("Description : %s\n",    description);
        printf("Author      : %s\n",    author);
        printf("Version     : %s\n",    version);
        printf("Built       : %s %s\n", __TIME__, __DATE__);
        printf("Compiler    : %s\n",    __VERSION__);
        printf("\n");
        bold(); printf("Copyright (c) %s %s\n", copyright_year, author); normal();
        printf("\n");
        printf("%s", MIT_LICENSE);
        printf("\n");
        printf("\n");

        exit(0);
    }

    void showHelp()
    {
        printf("\n");
        bold(); printf("NAME\n"); normal();
        printf("     %s - %s\n", program, description);
        printf("\n");
        bold(); printf("SYNOPSIS\n"); normal();
        printf("     %s [options] %s\n", name, args_help);
        printf("\n");
        bold(); printf("OPTIONS\n"); normal();

        OptionBase::printHelpAll();

        printf("\n");

        showExtraHelp();

        exit(0);
    }

protected:
    const char*    name;
    const char*    program;
    const char*    description;
    const char*    author;
    const char*    version;
    const char*    copyright_year;
    const char*    args_help;

    bool isDebug() const { return opt_debug; }

    void error(const char* format, ...)
    {
        va_list ap;

        fprintf(stderr, "%s: ERROR - ", name);

        va_start(ap, format);
        vfprintf(stderr, format, ap);
        va_end(ap);

        fprintf(stderr, "\n");

        exit(1);
    }

    virtual void showExtraHelp() {}

    virtual void parseArg(const char* arg) {}

    virtual int startConsoleApp() = 0;

public:
    ConsoleApp(const char*  program_,
               const char*  author_,
               const char*  description_,
               const char*  version_,
               const char*  copyright_year_,
               const char*  args_help_ = nullptr)
        : name(program_)
        , program(program_)
        , description(description_)
        , author(author_)
        , version(version_)
        , copyright_year(copyright_year_)
        , args_help(args_help_ ? args_help_ : "")
    {}

    void parseArgsAndStart(int argc, const char* argv[])
    {
#if defined(PROJ_TARGET_Emscripten)
        // TODO this is just a confidence test
        static const char* local_argv[] = {"fred", "-v"};

        argc = 2;
        argv = local_argv;
#endif
        name = extractFilename(argv[0]);

        for(int i=1; i<argc; ++i)
        {
            OptionBase* opt = OptionBase::find(argv[i]);
            if (opt)
            {
                const char* next_arg = (i + 1) < argc ? argv[i + 1] : nullptr;

                if (opt->set(next_arg))
                {
                   if (next_arg)
                   {
                      ++i;
                   }
                   else
                   {
                      error("option value for %s is missing", argv[i]);
                   }
                }
            }
            else if (argv[i][0] == '-')
            {
               error("unknown option %s", argv[i]);
            }
            else
            {
                parseArg(argv[i]);
            }
        }

        if (opt_version)  showVersion();
        if (opt_help)     showHelp();

        exit(startConsoleApp());
    }
};

} // namespace STB

#endif
