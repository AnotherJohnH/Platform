//------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
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

//! \file Socket.h
//! \brief Platform abstraction interface for network I/O

#ifndef PLT_SOCKET_H
#define PLT_SOCKET_H

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

#endif
