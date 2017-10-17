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

#ifndef STB_OPTION_H
#define STB_OPTION_H

#include <cstdio>
#include <cstring>

namespace STB {

//! Base class for command line options
class OptionBase
{
private:
   OptionBase* next = nullptr;
   char        short_opt;
   const char* long_opt;
   const char* description;

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
      if(ref == nullptr)
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
      printf("     ");

      if(short_opt != '\0')
      {
         printf("-%c", short_opt);
      }
      else
      {
         printf("  ");
      }

      if(long_opt != nullptr)
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
      if(arg_[0] != '-') return false;

      if(short_opt && (arg_[1] == short_opt)) return true;

      if(arg_[1] != '-') return false;

      return long_opt && (strcmp(arg_ + 2, long_opt) == 0);
   }

public:
   OptionBase(char short_opt_, const char* long_opt_, const char* description_)
      : short_opt(short_opt_)
      , long_opt(long_opt_)
      , description(description_)
   {
      addToList();
   }

   const char* getLongOpt() const { return long_opt; }
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
         if(opt->isMatch(arg_)) return opt;
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
   TYPE value;

   virtual bool set(const char* arg) override;

public:
   Option(char short_opt_, const char* long_opt_, const char* description_,
          const TYPE& default_value_ = TYPE())
      : OptionBase(short_opt_, long_opt_, description_)
      , value(default_value_)
   {}

   // Pretend option is TYPE

   operator const TYPE&() const { return value; }

   const TYPE& operator=(const TYPE& value_) { return value = value_; }
};

} // namespace STB

#endif
