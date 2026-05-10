//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "UCL/PrintF.h"

namespace {

class Buffer : public PrintF
{
public:
   Buffer() = default;

private:
   void putc(char ch) override
   {
      if (ch == '\n')
      {
         MTL_putch('\r');
      }
      MTL_putch(ch);
   }
};

void printf(const char* format, ...)
{
   Buffer  buffer{};
   va_list ap;

   va_start(ap, format);
   buffer.vprintf(format, ap);
   va_end(ap);
}

} // namespace

void MTL_fault(unsigned excep_num_, unsigned long* fault_stack_ptr_)
{
   // Prevent console out using IRQs (as they won't work)
   MTL_nobuff();

   printf("\n\nFAULT: #%u\n", excep_num_, fault_stack_ptr_[6]);

   printf("R0  = %08x  R1 = %08x  R2 = %08x  R3 = %08x\n",
          fault_stack_ptr_[0], fault_stack_ptr_[1],
          fault_stack_ptr_[2], fault_stack_ptr_[3]);

   printf("R12 = %08x                 LR = %08x  PC = %08x\n",
          fault_stack_ptr_[4], fault_stack_ptr_[5],
          fault_stack_ptr_[6]);

   printf("PSR = %08x\n", fault_stack_ptr_[7]);

   MTL_halt(excep_num_);
}
