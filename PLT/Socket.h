//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Socket.h
//! \brief Platform abstraction interface for network I/O

#pragma once

#include <cstddef>

namespace PLT {

//! A network socket
class Socket
{
public:
   Socket();

   ~Socket();

   //! Return true if the socket is open
   bool isOpen() const;

   //! Open socket as a network connection to a port on the named host
   bool connect(const char* hostname, unsigned port);

   //! Close a socket
   void close();

   //! Read data from a socket
   bool read(void* buffer, size_t buffer_size, size_t& bytes_read);

   //! Write data to a socket
   bool write(const void* buffer, size_t bytes_to_write);

   //! Helper to provide an old-style fgetc()
   int getchar()
   {
      uint8_t ch;
      size_t  bytes;

      if(read(&ch, 1, bytes))
      {
         if(bytes == 1)
         {
            return ch;
         }
      }

      return -1;
   }

   //! Helper to provide an old-style fgets()
   char* gets(char* buffer, size_t buffer_size)
   {
      size_t i=0;

      while(i<(buffer_size - 1))
      {
         int ch = getchar();
         if(ch < 0) return nullptr;

         buffer[i++] = ch;

         if (ch == '\n') break;
      }

      buffer[i] = '\0';

      return buffer;
   }

private:
   class Impl;

   Impl* pimpl{nullptr};
};

} // namespace PLT

