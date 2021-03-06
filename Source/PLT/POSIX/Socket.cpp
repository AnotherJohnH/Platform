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

#include <arpa/inet.h>
#include <cassert>
#include <cctype>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include "PLT/Socket.h"

namespace PLT {

//! A network socket
class Socket::Impl
{
public:
   Impl() = default;

   ~Impl()
   {
      close();
   }

   bool isOpen() const { return sd > 0; }

   //! Open a network connection to a port on a host
   bool connect(const char* hostname, unsigned port)
   {
      sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

      if (isOpen())
      {
         struct sockaddr_in addr;
         memset(&addr, 0, sizeof(addr));

         addr.sin_family      = AF_INET;
         addr.sin_port        = htons((u_short)port);
         addr.sin_addr.s_addr = getAddress(hostname);

         if(::connect(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
         {
            close();
         }
      }

      return isOpen();
   }

   //! Close a connection
   void close()
   {
      if (isOpen())
      {
         ::close(sd);
         sd = 0;
      }
   }

   bool read(void* buffer, size_t buffer_size, size_t& bytes_read)
   {
      ssize_t status = recv(sd, buffer, buffer_size, 0);
      if (status > 0)
      {
         bytes_read = status;
         return true;
      }

      bytes_read = 0;
      return false;
   }

   bool write(const void* buffer, size_t bytes_to_write)
   {
      return send(sd, buffer, bytes_to_write, 0) == ssize_t(bytes_to_write);
   }

private:
   int sd{0};

   in_addr_t getAddress(const char* hostname)
   {
      if (isdigit(hostname[0]))
      {
         return inet_addr(hostname);
      }

      struct hostent* hp = gethostbyname(hostname);
      if (hp != nullptr)
      {
          return *(in_addr_t*)hp->h_addr;
      }

      return INADDR_NONE;
   }
};


Socket::Socket()
   : pimpl(new Impl())
{
}

Socket::~Socket()
{
   delete pimpl;
}

bool Socket::isOpen() const
{
   return pimpl->isOpen();
}

bool Socket::connect(const char* hostname, unsigned port)
{
   return pimpl->connect(hostname, port);
}

void Socket::close()
{
   pimpl->close();
}

bool Socket::read(void* buffer, size_t buffer_size, size_t& bytes_read)
{
   return pimpl->read(buffer, buffer_size, bytes_read);
}

bool Socket::write(const void* buffer, size_t bytes_to_write)
{
   return pimpl->write(buffer, bytes_to_write);
}


} // namespace PLT
