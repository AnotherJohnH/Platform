//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>

namespace TST {

static const char* RED   = "\e[31m";
static const char* GREEN = "\e[32m";
static const char* WHITE = "\e[0m";

struct Func;

extern std::vector<Func*> test_list;

struct Func
{
   Func(const char* suite_,
            const char* name_,
            void (*func_)(bool&))
      : suite(suite_)
      , name(name_)
      , func(func_)
   {
      test_list.push_back(this);
   }

   bool run()
   {
      std::cout << GREEN << "[ RUN      ]" << WHITE << " " << suite << "." << name << "\n";

      (*func)(pass);

      if (pass)
         std::cout << GREEN << "[       OK ]";
      else
         std::cout << RED << "[  FAILED  ]";

      std::cout << WHITE << " " << suite << "." << name << "\n";

      return pass;
   }

   static void run_all()
   {
      std::cout << GREEN << "[==========]\n" << WHITE;

      size_t num_pass{0};

      for(auto& test : test_list)
      {
         if (test->run())
         {
            num_pass++;
         }
      }

      std::cout << GREEN << "[==========]\n" << WHITE;

      std::cout << GREEN << "[  PASSED  ]" << WHITE << " " << num_pass << " tests\n";

      size_t num_fail = test_list.size() - num_pass;

      if (num_fail)
      {
         std::cout << RED << "[  FAILED  ]" << WHITE << " " << num_fail << " tests\n";

         for(auto& test : test_list)
         {
            if (not test->pass)
               std::cout << RED << "[  FAILED  ]" << WHITE
                         << " " << test->suite << "." << test->name << "\n";
         }

         ::exit(EXIT_FAILURE);
      }
   }

   std::string suite;
   std::string name;
   bool        pass{true};
   void        (*func)(bool&);
};

inline void error(const char* file_, size_t line_)
{
   const char* slash = strrchr(file_, '/');
   file_ = slash != nullptr ? slash + 1 : file_;
   std::cout << file_ << "(" << line_ << "): error: ";
}

template <typename TYPE_L,
          typename TYPE_R>
inline bool expect_eq(TYPE_L      left_,
                      TYPE_R      right_,
                      const char* right_s_,
                      const char* file_,
                      size_t      line_)
{
   if (left_ == right_)
      return true;

   error(file_, line_);

   std::cout << "Value of " << right_s_ << "\n";
   std::cout << "  Actual: " << right_ << "\n";
   std::cout << "Expected: " << left_  << "\n";

   return false;
}

template <typename TYPE_L,
          typename TYPE_R>
inline bool expect_ne(TYPE_L      left_,
                      TYPE_R      right_,
                      const char* right_s_,
                      const char* file_,
                      size_t      line_)
{
   if (left_ != right_)
      return true;

   error(file_, line_);

   std::cout << "Value of " << right_s_ << "\n";
   std::cout << "Not expected: " << left_  << "\n";

   return false;
}

inline bool expect_streq(const char* left_,
                         const char* right_,
                         const char* right_s_,
                         const char* file_,
                         size_t      line_)
{
   if ((left_ == nullptr) && (right_ == nullptr))
      return true;

   if ((left_ != nullptr) && (right_ != nullptr) && (strcmp(left_, right_) == 0))
      return true;

   error(file_, line_);

   std::cout << "Value of " << right_s_ << "\n";
   std::cout << "  Actual: " << right_ << "\n";
   std::cout << "Expected: " << left_  << "\n";

   return false;
}

} // namespace Test

#define EXPECT_EQ(left, right) \
   pass_ &= TST::expect_eq(left, right, #right, __FILE__, __LINE__)

#define EXPECT_NE(left, right) \
   pass_ &= TST::expect_ne(left, right, #right, __FILE__, __LINE__)

#define EXPECT_TRUE(right) \
   pass_ &= TST::expect_eq(true, right, #right, __FILE__, __LINE__)

#define EXPECT_FALSE(right) \
   pass_ &= TST::expect_eq(false, right, #right, __FILE__, __LINE__)

#define EXPECT_STREQ(left, right) \
   pass_ &= TST::expect_streq(left, right, #right, __FILE__, __LINE__)

#define TEST(suite, name) \
   static void test_ ## suite ## _ ## name(bool& pass_); \
   static TST::Func func_ ## suite ## _ ## name{#suite, #name, \
      test_ ## suite ## _ ## name}; \
   static void test_ ## suite ## _ ## name(bool& pass_)

#define TEST_MAIN \
   std::vector<TST::Func*> TST::test_list{}; \
   int main() { TST::Func::run_all(); }