//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Http.h
//! \brief HTTP client

#pragma once

#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <functional>

#include "PLT/Socket.h"
#include "PLT/File.h"

namespace STB {

//! An HTTP client connection
class Http
{
public:
   Http() = default;

   //! Return true if the connection is open
   bool isOpen() const { return socket.isOpen(); }

   //! Open the HTTP connection to the named host
   bool open(const std::string& hostname_)
   {
      hostname = hostname_;
      return socket.connect(hostname_.c_str(), HTTP_PORT);
   }

   //! Close connection
   void close()
   {
      socket.close();
   }

   //! Send an HTTP request and receive the response
   unsigned request(std::function<void(const uint8_t*,size_t)> spooler,
                    const std::string&                         method,
                    const std::string&                         path,
                    const std::string&                         search = "")
   {
      // Request line
      std::string request = method + " ";

      request += path.empty() ? "/" : path;

      if(!search.empty())
      {
         request += "?";
         request += search;
      }

      request += " HTTP/1.1";
      request += HTTP_EOL();

      // Request header fields
      request += "Host: ";
      request += hostname;
      request += ":" + std::to_string(HTTP_PORT);
      request += HTTP_EOL();

      // Empty line
      request += HTTP_EOL();

      if(!socket.write(request.c_str(), request.size())) return 0;

      // Read status line
      char line[256];
      if (!socket.gets(line, sizeof(line))) return 0;

      // Extract status code
      char* space = strchr(line, ' ');
      if(space == nullptr) return false;
      unsigned status = strtoul(space + 1, nullptr, 10);

      unsigned content_length{0};
      bool     chunked{false};

      while(true)
      {
         // Read header field line
         if (!socket.gets(line, sizeof(line))) return 0;

         // Trim away end of the line
         char* eol = strstr(line, HTTP_EOL());
         if(eol == nullptr) return 0;
         *eol = '\0';

         // No more header fields exit to body
         if (strlen(line) == 0) break;

         // Parse interesting fields
         char* value = strchr(line, ':');
         if(value != nullptr)
         {
            *value++ = '\0';

            while(*value == ' ') { value++; }

            if (strcmp(line, "Content-Length") == 0)
            {
               content_length = strtoul(value, nullptr, 10);
            }
            else if (strcmp(line, "Transfer-Encoding") == 0)
            {
               chunked = strcmp(value, "chunked") == 0;
            }
         }
      }

      while(true)
      {
         if(chunked)
         {
            if (!socket.gets(line, sizeof(line))) return 0;
            content_length = strtoul(line, nullptr, 16);
            if (content_length == 0)
            {
               // Read final empty line
               if (!socket.gets(line, sizeof(line))) return 0;
               break;
            }
         }

         while(content_length > 0)
         {
            char   buffer[512];
            size_t bytes_read;

            size_t bytes_to_read = std::min(size_t(content_length), sizeof(buffer));
            if (!socket.read(buffer, bytes_to_read, bytes_read)) return 0;

            spooler((const uint8_t*)buffer, bytes_read);

            content_length -= bytes_read;
         }

         if(!chunked) break;

         if (!socket.gets(line, sizeof(line))) return 0;
      }

      return status;
   }

   //! Use HTTP to retrieve a file
   bool getFile(const std::string& path,
                const std::string& out_file)
   {
      STB::Http http;
      PLT::File file(nullptr, out_file.c_str());

      if(!file.openForWrite()) return false;

      unsigned status = request([&file](const uint8_t* buffer, size_t size)
                                {
                                   file.write(buffer, size);
                                },
                                "GET",
                                path);

      return 200 == status;
   }

private:
   static const unsigned HTTP_PORT = 80;

   const char* HTTP_EOL() const { return "\r\n"; }

   std::string hostname;
   PLT::Socket socket;
};

} // namespace PLT

