//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef STB_CSV_H
#define STB_CSV_H

#include <cassert>
#include <cstdio>

#include <algorithm>
#include <string>
#include <vector>


namespace STB {

namespace CSV {

template <typename FIELD_ID>
class Document
{
private:
   static const unsigned MAX_LINE_LENGTH = 512;

   struct Index
   {
      Index(size_t index_, FIELD_ID id_)
         : index(index_)
         , id(id_)
      {}

      inline bool operator<(const Index& rhs) const { return index < rhs.index; }

      size_t   index;
      FIELD_ID id;
   };

   FILE*                    fp{nullptr};
   std::vector<std::string> field_list;
   std::vector<Index>       index_list;

public:
   class AttrCallBack
   {
   protected:
      virtual ~AttrCallBack() = default;

   public:
      virtual void setAttr(FIELD_ID id, const std::string& value) = 0;
   };

   Document(const std::string& filename)
   {
      fp = fopen(filename.c_str(), "r");
   }

   ~Document()
   {
      if (fp != nullptr) fclose(fp);
   }

   bool isOpen() const { return fp != nullptr; }

   void readHeader()
   {
      assert(fp != nullptr);

      char line[MAX_LINE_LENGTH];
      if (fgets(line, sizeof(line), fp) == nullptr) return;

      std::string field;

      for(const char* s = line; true; s++)
      {
         if (*s == ',')
         {
            field_list.emplace_back(field);
            field.clear();
         }
         else if ((*s == '\n') || (*s == '\0'))
         {
            field_list.emplace_back(field);
            break;
         }
         else
         {
            field.push_back(*s);
         }
      }
   }

   //! Select a field to decode by index
   void requireField(size_t index, FIELD_ID id)
   {
      index_list.emplace_back(index, id);
      std::sort(index_list.begin(), index_list.end());
   }

   //! Select a field to decode by name
   bool requireField(const std::string& name, FIELD_ID id)
   {
      size_t index = 0;

      for(auto& field : field_list)
      {
         if (field == name)
         {
            requireField(index, id);
            return true;
         }

         index++;
      }

      return false;
   }

   void debugFields() const
   {
      unsigned index = 0;

      typename std::vector<Index>::const_iterator it = index_list.cbegin();

      for(const auto& field : field_list)
      {
         printf("%2u \"%s\"", index + 1, field.c_str());

         if (it->index == index)
         {
            printf(" REQ");
            ++it;
         }

         printf("\n");
         ++index;
      }
   }

   bool readRecord(AttrCallBack& call_back) const
   {
      assert(fp != nullptr);

      char line[MAX_LINE_LENGTH];

      if (fgets(line, sizeof(line), fp) == nullptr) return false;

      size_t index = 0;
      const char* start = line;
      typename std::vector<Index>::const_iterator it = index_list.cbegin();

      for(char* s = line; it != index_list.cend(); s++)
      {
         if (*s == ',')
         {
            *s = '\0';

            if (it->index == index)
            {
               call_back.setAttr(it->id, start);
               ++it;
            }

            start = s + 1;
            index++;
         }
         else if ((*s == '\n') || (*s == '\0'))
         {
            *s = '\0';

            if (it->index == index)
            {
               call_back.setAttr(it->id, start);
               ++it;
            }
            break;
         }
      }

      return it == index_list.cend();
   }
};

} // namespace CSV

} // namespace STB

#endif // STB_CSV_H
