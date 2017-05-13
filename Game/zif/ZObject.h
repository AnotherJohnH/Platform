//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef ZOBJECT_H
#define ZOBJECT_H

#include <cassert>

#include "ZMemory.h"

class ZObject
{
private:
   static const unsigned SMALL_PROP_BITS = 5;
   static const unsigned LARGE_PROP_BITS = 6;

   ZMemory*    memory;
   uint16_t    obj_table;    //!< Address of object table
   bool        small;        //!< true => Small table v1 to v3

   unsigned getPropBits() const { return small ? SMALL_PROP_BITS : LARGE_PROP_BITS; }
   unsigned getMaxProps() const { return (1<<getPropBits()) - 1; }
   uint16_t getMaxObjs()  const { return small ? 255 : 65535; }
   unsigned getMaxAttr()  const { return small ?  32 :    48; }
   unsigned getObjSize()  const { return small ?   9 :    14; }

   //! Return the default value for a property
   uint16_t getDefaultProp(unsigned index) const
   {
      assert((index > 0) && (index <= getMaxProps()));

      return memory->readWord(obj_table + (index - 1) * sizeof(uint16_t));
   }

   //! Return the address for the given object index
   uint32_t getObjAddress(uint16_t index) const
   {
      assert((index > 0) && (index <= getMaxObjs()));

      return obj_table + getMaxProps()*sizeof(uint16_t) + (index - 1) * getObjSize();
   }

   //! Return the nth object link (0=>parent, 1=>sibling, 2=>child)
   uint16_t getObjLink(uint16_t obj, unsigned n) const
   {
      if (obj == 0) return 0;  // XXX seems this is ok!

      uint32_t addr = getObjAddress(obj) + (getMaxAttr() / 8);
      if (small)
         return memory->readByte(addr + n);
      else
         return memory->readWord(addr + n * sizeof(uint16_t));
   }

   //! Set the nth object link (0=>parent, 1=>sibling, 2=>child)
   void setObjLink(uint16_t obj, unsigned n, uint16_t link)
   {
      uint32_t addr = getObjAddress(obj) + (getMaxAttr() / 8);
      if (small)
         memory->writeByte(addr + n, link);
      else
         memory->writeWord(addr + n * sizeof(uint16_t), link);
   }

   unsigned fetchPropInfo(uint32_t& addr, unsigned& index) const
   {
      uint8_t id = memory->readByte(addr++);
      if (id == 0)
      {
         index = 0;
         return 0;
      }

      index = id & getMaxProps();

      unsigned size;

      if (small)
      {
         size  = (id >> SMALL_PROP_BITS) + 1;
      }
      else
      {
         if (id & (1<<7))
         {
            size = memory->readByte(addr++) & 0x3F;
            if (size == 0)
            {
               size = 0x40;
            }
         }
         else
         {
            size = (id >> LARGE_PROP_BITS) + 1;
         }
      }

      assert(index != 0);

      //printf("\ngetPropInfo %d %04X %d\n", index, addr, size);

      return size;
   }

   uint32_t findProp(uint16_t obj, unsigned prop, unsigned& size) const
   {
      uint32_t addr = getPropTableAddress(obj);

      // Skip object name
      uint8_t  name_len = memory->readByte(addr++);
      addr += name_len * 2;

      while(true)
      {
         unsigned p;
         size = fetchPropInfo(addr, p);
         if (size == 0)
         {
            break;
         }
         else if (p == prop)
         {
            return addr;
         }
         else if (p < prop)
         {  // Not found (properties are stored in decending order)
            break;
         }

         addr += size;
      }

      size = 0;
      return 0;
   }

public:
   ZObject(ZMemory* memory_)
      : memory(memory_)
      , obj_table(0)
      , small(true)
   {
   }

   //! Initialise with game information
   void init(uint16_t obj_table_, uint8_t version_)
   {
      obj_table = obj_table_;
      small     = (version_ >= 1) && (version_ <= 3);
   }

   //! Return the state of an objects attribute
   bool getAttr(uint16_t obj, unsigned attr) const
   {
      if (obj == 0) return false;  // XXX seems this is ok!

      assert(attr < getMaxAttr());

      uint32_t addr = getObjAddress(obj) + (attr/8);
      unsigned bit  = 7 - (attr & 7);
      uint8_t  byte = memory->readByte(addr);
      return (byte & (1<<bit)) != 0;
   }

   //! Set the state of an objects attribute
   void setAttr(uint16_t obj, unsigned attr, bool set)
   {
      if (obj == 0) return;  // XXX seems this is ok!

      assert(attr < getMaxAttr());

      uint32_t addr = getObjAddress(obj) + (attr/8);
      unsigned bit  = 7 - (attr & 7);
      uint8_t  byte = memory->readByte(addr);
      if (set)
         byte |= 1<<bit;
      else
         byte &= ~(1<<bit);
      memory->writeByte(addr, byte);
   }

   uint16_t getParent( uint16_t obj) const { return getObjLink(obj, 0); }
   uint16_t getSibling(uint16_t obj) const { return getObjLink(obj, 1); }
   uint16_t getChild(  uint16_t obj) const { return getObjLink(obj, 2); }

   void setParent( uint16_t obj, uint16_t parent)  { setObjLink(obj, 0, parent);  }
   void setSibling(uint16_t obj, uint16_t sibling) { setObjLink(obj, 1, sibling); }
   void setChild(  uint16_t obj, uint16_t child)   { setObjLink(obj, 2, child);   }

   uint32_t getPropTableAddress(uint16_t obj) const
   {
      uint32_t addr = getObjAddress(obj);
      return small ? memory->readWord(addr + 7)
                   : memory->readWord(addr + 12);
   }

   uint32_t getName(uint16_t obj) const
   {
      return getPropTableAddress(obj) + 1;
   }

   uint32_t getPropAddr(uint16_t obj, unsigned prop) const
   {
      if (obj == 0) return 0;  // XXX seems this is ok!

      unsigned size;
      return findProp(obj, prop, size);
   }

   uint32_t getPropNext(uint16_t obj, unsigned prop) const
   {
      if (obj == 0) return 0;  // XXX seems this is ok!

      uint32_t addr = getPropTableAddress(obj);

      // Skip object name
      uint8_t  name_len = memory->readByte(addr++);
      addr += name_len * 2;

      if (prop == 0)
      {
         unsigned p;
         (void) fetchPropInfo(addr, p);
         return p;
      }

      while(true)
      {
         unsigned p;
         unsigned size = fetchPropInfo(addr, p);
         if (size == 0)
         {
            break;
         }
         else if ((prop == 0) || (p < prop))
         {
            // properties are stored in decending order
            return p;
         }

         addr += size;
      }

      return 0;
   }

   unsigned propSize(uint32_t addr) const
   {
      --addr;
      if (!small)
      {
         uint8_t id = memory->readByte(addr);
         if ((id & (1<<7)) != 0)
         {
            --addr;
         }
      }
      unsigned index;
      return fetchPropInfo(addr, index);
   }

   uint16_t getProp(uint16_t obj, unsigned prop) const
   {
      if (obj == 0) return 0;  // XXX seems this is ok!

      unsigned size;
      uint32_t addr = findProp(obj, prop, size);
      switch(size)
      {
      case 0:  return getDefaultProp(prop);
      case 1:  return memory->readByte(addr);
      default: return memory->readWord(addr);
      }
      return 0;
   }

   void setProp(uint16_t obj, unsigned prop, uint16_t value)
   {
      if (obj == 0) return;  // XXX seems this is ok!

      unsigned size;
      uint32_t addr = findProp(obj, prop, size);
      switch(size)
      {
      case 0:  assert(!"property not found");
      case 1:  return memory->writeByte(addr, uint8_t(value));
      case 2:  return memory->writeWord(addr, value);
      default: assert(!"property size must be 1 or 2");
      }
   }

   //! Remove an object from the tree
   void remove(uint16_t obj)
   {
      if (obj == 0) return;  // XXX seems this is ok!

      uint16_t parent = getParent(obj);
      if (parent)
      {
         uint16_t prev = 0;
         for(uint16_t o = getChild(parent); o; o = getSibling(o))
         {
            if (o == obj)
            {
               if (prev == 0)
               {
                  setChild(parent, getSibling(obj));
               }
               else
               {
                  setSibling(prev, getSibling(obj));
               }
               setParent(obj, 0);
               setSibling(obj, 0);
               return;
            }

            prev = o;
         }

         assert(!"object hierarchy is corrupt");
      }
   }

   //! Insert an object into the tree
   void insert(uint16_t obj, uint16_t parent)
   {
      if (obj == 0 || parent==0) return;  // XXX seems this is ok!

      // remove object from pre-exiting container
      remove(obj);

      setParent(obj, parent);
      setSibling(obj, getChild(parent));
      setChild(parent, obj);
   }
};

#endif
