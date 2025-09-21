//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
   static const char DEC_DIGIT   = 1;
   static const char OCT_DIGIT   = 2;
   static const char HEX_DIGIT   = 3;
   static const char IDENT_START = 4;
   static const char IDENT_BODY  = 5;
   static const char SIGN        = 6;

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
   SmallLex(const char* filename, const char* ext = nullptr)
      : PLT::File(nullptr, filename, ext)
   {
      (void) openForRead();
   }

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
         case DEC_DIGIT:   is_match = isdigit(ch); break;
         case OCT_DIGIT:   is_match = ((ch >= '0') && (ch <= '7')); break;
         case HEX_DIGIT:   is_match = isdigit(ch) || ((ch >= 'A') && (ch <= 'F'))
                                                  || ((ch >= 'a') && (ch <= 'f')); break;
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
      return doMatch("\4\5*", "identifier", err);
   }

   //! Match an unsigned integer 
   bool matchUnsigned(bool err=true)
   {
      if (doMatch("0x\3+", "unsigned number", false))
      {
         return true;
      }
      else if (doMatch("0\2+", "unsigned number", false))
      {
         return true;
      }
      else
      {
         return doMatch("\1+", "unsigned number", err);
      }
   }

   //! Match a signed integer
   bool matchSigned(bool err=true)
   {
      return doMatch("\6?\1+", "signed number", err);
   }

   //! Match a floating point number
   bool matchFloat(bool err=true)
   {
      return doMatch("\6?\1+.\1+", "float-point number", err);
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
