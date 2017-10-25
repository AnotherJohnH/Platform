//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

#ifndef STB_SMALL_LEX_H
#define STB_SMALL_LEX_H

#include <cctype>
#include <string>
#include <vector>

#include "PLT/File.h"

namespace STB {

//! A small and simple lexical analyser
class SmallLex : public PLT::File
{
private:
   static const char DIGIT       = 1;
   static const char IDENT_START = 2;
   static const char IDENT_BODY  = 3;
   static const char SIGN        = 4;

   bool getc(char& ch)
   {
      if (unget.size() > 0)
      {
         ch = unget.back();
         unget.pop_back();
         return true;
      }

      return getChar(ch);
   }

   bool pop(char& ch, bool first)
   {
      if (first && (pos == buffer.size()))
      {
         // Skip white space
         while(true)
         {
            if (!getc(ch)) return false;

            if ((ch != ' ') && (ch != '\n')) break;
         }

         buffer = ch;
         pos = 1;
      }
      else if (pos < buffer.size())
      {
         ch = buffer[pos++];
      }
      else
      {
         if (!getc(ch)) return false;

         buffer += ch;
         pos = buffer.size();
      }

      return true;
   }

   void push()
   {
      --pos;
   }

   void setNoMatch()
   {
      pos = 0;
   }

   void setMatch()
   {
      while(pos < buffer.size())
      {
         char ch = buffer.back();
         buffer.pop_back();
         unget.push_back(ch);
      }
   }

public:
   SmallLex(const char* filename)
      : PLT::File(filename, "r")
   {}

   SmallLex(const char* filename, const char* ext)
      : PLT::File(filename, ext, "r")
   {}

   const char* getToken() const { return buffer.c_str(); }

   bool doMatch(const char* token, const char* description, bool err)
   {
      bool is_regex = description != nullptr;

      char  ch;

      if (!pop(ch, /* first */ true)) return false;

      bool is_match = true;
      bool matched_one = false;

      for(const char* t = token; *t;)
      {
         const char* loop = t;

         switch(*t)
         {
         case DIGIT:       is_match = isdigit(ch); break;
         case IDENT_START: is_match = isalpha(ch) || (ch == '_'); break;
         case IDENT_BODY:  is_match = isalpha(ch) || isdigit(ch) || (ch == '_'); break;
         case SIGN:        is_match = (ch == '+') || (ch == '-'); break;
         case '\\':        is_match = ch == *(++t); break;
         default:          is_match = ch == *t; break;
         }

         char next = *(++t);

         if (next == '*')
         {
            if (is_match)
            {
               t = loop;
            }
            else
            {
               t++;
               push();
               is_match = true;
               continue;
            }
         }
         else if (next == '+')
         {
            if (is_match)
            {
               matched_one = true;
               t = loop;
            }
            else if (matched_one)
            {
               matched_one = false;
               t++;
               push();
               is_match = true;
               continue;
            }
            else
            {
               break;
            }
         }
         else if (next == '?')
         {
            t++;
            if (!is_match)
            {
               continue;
            }
         }
         else if (next == '\0')
         {
            break;
         }
         else if (!is_match)
         {
            break;
         }

         if (!pop(ch, /* first */ false))
         {
            is_match = false;
            break;
         }
      }

      if (!is_match)
      {
         if (err)
         {
            if (is_regex)
               error("Expected %s got '%s'", description, getToken());
            else
               error("Expected '%s' got '%s'", token, getToken());
         }
         else
         {
            setNoMatch();
         }
      }
      else
      {
         setMatch();
      }

      return is_match;
   }

   //! Match a string
   bool match(const char* token, bool err=true)
   {
      return doMatch(token, nullptr, err);
   }

   //! Match a single character
   bool match(char token, bool err=true)
   {
      char token_str[2] = { token, '\0'};
      return doMatch(token_str, nullptr, err);
   }

   //! Match an identifier
   bool matchIdent(bool err=true)
   {
      return doMatch("\2\3*", "identifier", err);
   }

   //! Match an unsigned integer 
   bool matchUnsigned(bool err=true)
   {
      return doMatch("\1+", "unsigned number", err);
   }

   //! Match a signed integer
   bool matchSigned(bool err=true)
   {
      return doMatch("\4?\1+", "signed number", err);
   }

   //! Match a floating point number
   bool matchFloat(bool err=true)
   {
      return doMatch("\4?\1+.\1+", "float-point number", err);
   }

   bool tryMatch(const char* token) { return match(token, false);  }
   bool tryMatch(char token)        { return match(token, false);  }
   bool tryMatchIdent()             { return matchIdent(false);    }
   bool tryMatchUnsigned()          { return matchUnsigned(false); }
   bool tryMatchSigned()            { return matchSigned(false);   }
   bool tryMatchFloat()             { return matchFloat(false);    }

private:
   std::string        buffer;
   size_t             pos{0};
   std::vector<char>  unget;
};

} // namespace STB

#endif
