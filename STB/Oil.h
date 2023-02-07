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

#ifndef STB_OIL_H
#define STB_OIL_H

#include <vector>
#include <typeinfo>
#include <cassert>
#include <cstring>
#include <cstdint>

namespace PLT { class File; }

namespace STB {

class Lex;

namespace OIL {

//! Describe a class member
class Member
{
public:
   using Flags = uint32_t;

   static const Flags HEX = 1<<0;

   enum class Type
   {
      BOOL,
      ENUM,
      SIGNED,
      UNSIGNED,
      FLOAT
   };

   Member(Type        type_,
          size_t      size_,
          size_t      offset_,
          size_t      elements_,
          const char* name_)
      : type(type_)
      , size(size_)
      , offset(offset_)
      , elements(elements_)
      , name(name_)
   {}

   bool isCalled(const char* name_) const
   {
      return strcmp(name, name_) == 0;
   }

   void setFlags(Flags mask)
   {
      flags |= mask;
   }

   //! Write data member to a file stream
   void write(PLT::File& file, void* that) const;

   //! Read a data member value from a file stream
   bool read(Lex& lex, void* that) const;

private:
   Type        type;     //!< Type
   size_t      size;     //!< Data size (bytes)
   size_t      offset;   //!< Offset into the object (bytes)
   size_t      elements; //!< Number of elements
   const char* name;     //!< Name
   Flags       flags{0};
};


//! Class to help determine array element type without running into SFINAE problems
//  in ClassBase::addMember
template <typename TYPE, bool is_array>
struct Element;

template <typename TYPE>
struct Element<TYPE,false>
{
   using ELEMENT = TYPE;
};

template <typename TYPE>
struct Element<TYPE,true>
{
   TYPE tmp; // TODO avoid this declaration

   using ELEMENT = typename std::remove_reference<decltype(tmp[0])>::type;
};


//! Class description for Oil objects
class ClassBase
{
public:
#ifndef NO_RTTI
   ClassBase(const char* name_,
             const std::type_info& type_info_)
      : name(name_)
      , hash(type_info_.hash_code())
   {
      getClassList().push_back(this);
   }
#endif

   bool isCalled(const char* name_) const
   {
      return strcmp(name, name_) == 0;
   }

   template <typename TYPE>
   void addMember(size_t offset, const char* name, size_t elements=1)
   {
#ifndef NO_RTTI
      if (typeid(TYPE) == typeid(bool))
      {
         member_list.push_back(Member(Member::Type::BOOL, sizeof(TYPE), offset, elements, name));
      }
      else if (std::is_floating_point<TYPE>::value)
      {
         member_list.push_back(Member(Member::Type::FLOAT, sizeof(TYPE), offset, elements, name));
      }
      else if (std::is_enum<TYPE>::value)
      {
         member_list.push_back(Member(Member::Type::ENUM, sizeof(TYPE), offset, elements, name));
      }
      else if (std::is_array<TYPE>::value)
      {
         using ELEMENT = typename Element<TYPE,std::is_array<TYPE>::value>::ELEMENT;

         addMember<ELEMENT>(offset, name, sizeof(TYPE) / sizeof(ELEMENT));
      }
      else if (std::is_integral<TYPE>::value)
      {
         if (std::is_signed<TYPE>::value)
         {
            member_list.push_back(Member(Member::Type::SIGNED, sizeof(TYPE), offset, elements, name));
         }
         else
         {
            member_list.push_back(Member(Member::Type::UNSIGNED, sizeof(TYPE), offset, elements, name));
         }
      }
#endif
   }

   bool exists(void* that) const;

   void write(void* that) const;

   bool read(void* that) const;

   template <typename TYPE>
   static ClassBase* findClass()
   {
#ifndef NO_RTTI
      size_t hash = typeid(TYPE).hash_code();

      for(auto& c : getClassList())
      {
         if (hash == c->hash) return c;
      }
#endif

      assert(!"Class not found");
      return nullptr;
   }

protected:
   void flags(const char* name, Member::Flags mask)
   {
      Member* member = findMember(name);
      if (member)
      {
         member->setFlags(mask);
      }
   }

private:
   void write(PLT::File& file, void* that) const;

   bool read(Lex& lex, void* that) const;

   Member* findMember(const char* name)
   {
      for(auto& m : member_list)
      {
         if (m.isCalled(name)) return &m;
      }

      return nullptr;
   }

   const Member* findMember(const char* name) const
   {
      for(const auto& m : member_list)
      {
         if (m.isCalled(name)) return &m;
      }

      return nullptr;
   }

   static std::vector<ClassBase*>& getClassList()
   {
      static std::vector<ClassBase*> list;
      return list;
   }

   const char*         name;
   size_t              hash;
   std::vector<Member> member_list;
};


//! Oil class description for objects of type TYPE
template <typename TYPE>
class Class : public ClassBase
            , public TYPE
{
public:
   Class(const char* name_)
#ifndef NO_RTTI
      : ClassBase(name_, typeid(TYPE))
#endif
   {}

   //! Declare a member
   template <typename MEMBER>
   void member(const MEMBER& m, const char* name)
   {
      size_t offset = uintptr_t(&m) - uintptr_t((TYPE*)this);

      ClassBase::addMember<MEMBER>(offset, name);
   }
};

} // namespace OIL


//! Base class for Oil object of type TYPE
template <typename TYPE>
class Oil
{
public:
   Oil()
      : oil_class(OIL::ClassBase::findClass<TYPE>())
   {}

   bool exists() { return oil_class->exists(this); }

   bool read() { return oil_class->read(this); }

   void write() { oil_class->write(this); }

private:
   OIL::ClassBase* oil_class{nullptr};
};


} // namespace STB 


#define  BOIL(C) static struct C ## Class : public STB::OIL::Class<C> \
                    { \
                       C ## Class () : STB::OIL::Class<C>(#C)

#define  MOIL(M) member(M, #M);

#define  FOIL(M,F) flags(#M, STB::OIL::Member::F);

#define  EOIL(C) } C ## _class;


#endif
