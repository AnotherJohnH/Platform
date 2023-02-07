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

#include <string>

#include "SingletonList.h"

namespace STB {

//! Base class for command line options
class OptionBase : public SingletonList::Element<OptionBase,
                                                 /* CONSTRUCTION_ORDER */true>
{
public:
   OptionBase(char        short_opt_,
              const char* long_opt_,
              const char* description_)
#if !defined(PLT_NCONSOLE)
      : description(description_)
      , long_opt(long_opt_)
      , short_opt(short_opt_)
#endif
   {
   }

#if !defined(PLT_NCONSOLE)
   //! Print help for all the options
   static void printHelpAll()
   {
      for(const OptionBase* option = front(); option; option = option->next())
      {
         if (!option->isGlob())
         {
            option->printHelp();
         }
      }
   }

   //! Find an option matching a command line argument
   static OptionBase* find(const char* arg_)
   {
      for(OptionBase* option = front(); option; option = option->next())
      {
         if(option->isMatch(arg_)) return option;
      }

      return nullptr;
   }

   //! Get the help suffix used to describe general arguments
   static const char* getSynopsisSuffix()
   {
      for(OptionBase* option = front(); option; option = option->next())
      {
         if (option->isGlob())
         {
            return option->description;
         }
      }

      return "";
   }

   //! Is glob
   bool isGlob() const { return short_opt == '*'; }
#endif

   //! Set option value from a string
   virtual bool set(const char* arg) = 0;

private:
   //! Report the default value on the console
   virtual void showDefault() const = 0;

   //! Return a description of the supplementary option value
   virtual const char* getValueDescription() const = 0;

#if !defined(PLT_NCONSOLE)
   //! Print option help
   void printHelp() const
   {
      std::string syntax;

      if(short_opt != '\0')
      {
         syntax += '-';
         syntax += short_opt;
         if (long_opt) syntax += ',';
      }

      if(long_opt != nullptr)
      {
         syntax += "--";
         syntax += long_opt;
      }

      const char* value_descr = getValueDescription();
      if(value_descr != nullptr)
      {
         syntax += ' ';
         syntax += value_descr;
      }

      printf("     %-24s %s", syntax.c_str(), description);

      showDefault();

      printf("\n");
   }

   //! Check if the given command line argument matches this option
   bool isMatch(const char* arg_) const
   {
      if(arg_[0] == '-')
      {
         if((short_opt != '\0') && (arg_[1] == short_opt))
         {
            return true;
         }

         if(arg_[1] == '-')
         {
            return (long_opt != nullptr) && (strcmp(arg_ + 2, long_opt) == 0);
         }
         else
         {
            return false;
         }
      }
      else
      {
         return isGlob();
      }
   }

   const char* description;
   const char* long_opt;
   char        short_opt;
#endif
};


//! Command line option
template <typename TYPE>
class Option : public OptionBase
{
public:
   Option(char        short_opt_,
          const char* long_opt_,
          const char* description_,
          const TYPE& default_value_ = TYPE())
      : OptionBase(short_opt_, long_opt_, description_)
      , value(default_value_)
   {}

   const TYPE& get() const { return value; }

   operator const TYPE&() const { return value; }

   const TYPE& operator=(const TYPE& value_) { return value = value_; }

protected:
   bool set(const char* arg) override;

private:
   void showDefault() const override;

   const char* getValueDescription() const override;

   TYPE value{};
};

} // namespace STB

#endif
