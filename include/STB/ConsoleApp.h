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

namespace STB {

//! Base class for command line options
class OptionBase
{
private:
   OptionBase*   next = nullptr;
   char          short_opt;
   const char*   long_opt;
   const char*   description;

   //! Access to the global list of options
   static OptionBase*& getSingletonList()
   {
      static OptionBase* list = nullptr;
      return list;
   }

   //! Add an option to the end of the global list
   void addToList()
   {
      OptionBase* ref = getSingletonList();
      if (ref == nullptr)
      {
         getSingletonList() = this;
      }
      else
      {
         while(ref->next)
         {
            ref = ref->next;
         }
         ref->next = this;
      }
   }

   //! Print option help
   void printHelp() const
   {
      printf("        ");

      if (short_opt != '\0')
      {
         printf("-%c", short_opt);
      }
      else
      {
         printf("  ");
      }

      if (long_opt != nullptr)
      {
         printf("%c", short_opt ? ',' : ' ');
         printf(" --%-15s", long_opt);
      }
      else
      {
         printf("%-19s", "");
      }

      printf(" %s\n", description);
   }

   //! Check if the given command line argument matches this option
   bool isMatch(const char* arg_) const
   {
      if (arg_[0] != '-') return false;

      if (short_opt && (arg_[1] == short_opt)) return true;

      if (arg_[1] != '-') return false;

      return long_opt && (strcmp(arg_ + 2, long_opt) == 0);
   }

public:
   OptionBase(char        short_opt_,
              const char* long_opt_,
              const char* description_)
      : short_opt(short_opt_)
      , long_opt(long_opt_)
      , description(description_)
   {
      addToList();
   }

   const char* getLongOpt()  const { return long_opt; }
   char        getShortOpt() const { return short_opt; }

   //! Print help for all the options
   static void printHelpAll()
   {
      for(const OptionBase* opt = getSingletonList(); opt; opt = opt->next)
      {
         opt->printHelp();
      }
   }

   //! Find an option from a command line argument
   static OptionBase* find(const char* arg_)
   {
      for(OptionBase* opt = getSingletonList(); opt; opt = opt->next)
      {
         if (opt->isMatch(arg_)) return opt;
      }

      return nullptr;
   }

   //! Set option value from a string
   virtual bool set(const char* arg) = 0;
};


//! Command line option
template <typename TYPE>
class Option : public OptionBase
{
private:
   TYPE   value;

   virtual bool set(const char* arg) override;

public:
   Option(char        short_opt_,
          const char* long_opt_,
          const char* description_,
          const TYPE& default_value_ = TYPE())
      : OptionBase(short_opt_, long_opt_, description_)
      , value(default_value_)
   {}

   // Pretend option is TYPE

   operator const TYPE&() const { return value; }

   const TYPE& operator=(const TYPE& value_) { return value = value_; }
};


template <>
bool Option<bool>::set(const char* arg_)
{
   value = true;
   return false;
}

template <>
bool Option<int>::set(const char* arg_)
{
   if (arg_ != nullptr) value = strtol(arg_, nullptr, 0);
   return true;
}

template <>
bool Option<unsigned>::set(const char* arg_)
{
   if (arg_ != nullptr) value = strtoul(arg_, nullptr, 0);
   return true;
}

template <>
bool Option<const char*>::set(const char* arg_)
{
   if (arg_ != nullptr) value = arg_;
   return true;
}


//! A generic class to simplify the creation of user friendly console applications
class ConsoleApp
{
private:
    static const char* extractFilename(const char* path)
    {
       // Windows is not supported
       const char* filename = strrchr(path, '/');

       return filename != nullptr ? filename + 1
                                  : path;
    }

    const char*    name;
    const char*    program;
    const char*    description;
    const char*    author;
    const char*    version;
    const char*    copyright_year;
    const char*    license;
    const char*    args_help;

protected:
    unsigned       argc;
    const char**   argv;

    Option<bool>   opt_version;
    Option<bool>   opt_help;
    Option<bool>   opt_debug;

public:
    ConsoleApp(int          argc_,
               const char*  argv_[],
               const char*  program_,
               const char*  author_,
               const char*  description_,
               const char*  version_,
               const char*  copyright_year_,
               const char*  license_,
               const char*  args_help_ = nullptr)
        : name(extractFilename(argv_[0]))
        , program(program_)
        , description(description_)
        , author(author_)
        , version(version_)
        , copyright_year(copyright_year_)
        , license(license_)
        , args_help(args_help_ ? args_help_ : "")
        , argc(argc_)
        , argv(argv_)
        , opt_version('v', "version", "Display version information")
        , opt_help(   'h', "help",    "Display this help")
        , opt_debug(  'd', "debug",   "Report extra debug information")
    {}

    bool isDebug() const { return opt_debug; }

    void parseArgsAndStart()
    {
        unsigned j = 1;

        for(unsigned i=1; i<argc; ++i)
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
                argv[j++] = argv[i];
            }
        }

        argc = j;

        if (opt_version)  showVersion();
        if (opt_help)     showHelp();

        exit(start());
    }

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
        printf("Copyright (c) %s %s\n", copyright_year, author);
        printf("\n");
        printf("%s", license);

        exit(0);
    }

    void showHelp()
    {
        printf("\n");
        printf("NAME\n");
        printf("        %s - %s\n", program, description);
        printf("\n");
        printf("SYNOPSIS\n");
        printf("        %s [options] %s\n", name, args_help);
        printf("\n");
        printf("OPTIONS\n");

        OptionBase::printHelpAll();

        printf("\n");

        extraHelp();

        exit(0);
    }

    virtual void extraHelp() {}

    virtual int start() = 0;
};

} // namespace STB

#endif
