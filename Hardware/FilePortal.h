//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "STB/FAT/FAT16.h"

namespace hw {

class FilePortal : public STB::FAT16<6>
{
public:
   FilePortal(const char* fs_name_,
              const char* url_)
      : STB::FAT16<6>(fs_name_)
   {
      addINDEX(url_);
   }

   //! Auto-generate the project README
   const char* addREADME(const char* program_)
   {
      char* s = readme_txt;
      const char* end_s = s + sizeof(readme_txt) - 1;

      s += snprintf(s, end_s - s, "Program  : %s (%s)\n", program_, HW_DESCR);
      s += snprintf(s, end_s - s, "Author   : Copyright (c) 2025 John D. Haughton\n");
      s += snprintf(s, end_s - s, "License  : MIT\n");
      s += snprintf(s, end_s - s, "Version  : %s\n", PLT_VERSION);
      s += snprintf(s, end_s - s, "Commit   : %s\n", PLT_COMMIT);
      s += snprintf(s, end_s - s, "Built    : %s %s\n", __TIME__, __DATE__);
#if defined(__clang__)
      s += snprintf(s, end_s - s, "Compiler : Clang %s\n", __VERSION__);
#elif defined(__GNUC__)
      s += snprintf(s, end_s - s, "Compiler : GCC %s\n", __VERSION__);
#else
      s += snprintf(s, end_s - s, "Compiler : %s\n", __VERSION__);
#endif

#if not defined(HW_NATIVE)
      s += MTL::config.format(s, end_s - s);
#endif

      addFile("README.txt", strlen(readme_txt), (uint8_t*)readme_txt);

      return readme_txt;
   }

private:
   //! Auto-generate the project INDEX.html
   void addINDEX(const char* url_)
   {
      char* s = index_html;
      const char* end_s = s + sizeof(index_html) - 1;

      s += snprintf(s, end_s - s,
         "<html>"
            "<head>"
               "<meta http-equiv=\"refresh\" content=\"0;URL='%s'\"/>"
            "</head>"
            "<body>"
               "Redirecting to <a href='%s'>github.com</a>"
            "</body>"
         "</html>",
         url_, url_);

      addFile("INDEX.htm", strlen(index_html), (uint8_t*)index_html);
   }

   char readme_txt[2048];
   char index_html[512];
};

} // namespace hw
