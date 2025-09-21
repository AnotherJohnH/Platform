//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <fcntl.h>
#include <linux/fb.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include "PLT/Frame.h"

namespace PLT {

class Frame::Impl
{
private:
   unsigned width{600};
   unsigned height{800};
   unsigned pitch{300};
   uint8_t* buffer{nullptr};
   int      refresh_fd{-1};

   size_t getSize() const { return pitch * height; }

   static void error(const char* format, ...)
   {
      va_list ap;

      fprintf(stderr, "ERROR - ");

      va_start(ap, format);
      vfprintf(stderr, format, ap);
      va_end(ap);

      fprintf(stderr, "\n");
      exit(1);
   }

   static int openDev(const char* filename)
   {
      int fd = open(filename, O_RDWR);
      if(-1 == fd)
      {
         error("Failed to open \"%s\"", filename);
      }
      return fd;
   }

public:
   Impl()
   {
      int fd = openDev("/dev/fb0");

      struct fb_var_screeninfo screeninfo;
      int                      status = ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo);
      if(-1 == status)
      {
         error("ioctl(.. FBIOGET_VSCREENINFO) failed");
      }

      width  = screeninfo.xres;
      height = screeninfo.yres;
      pitch  = (width + 1)/2;

      buffer = (uint8_t*)mmap(nullptr, getSize(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if(nullptr == buffer)
      {
         error("mmap() failed");
      }

      close(fd);

      refresh_fd = openDev("/proc/eink_fb/update_display");
   }

   ~Impl()
   {
      munmap(buffer, getSize());
      close(refresh_fd);
   }

   unsigned getWidth() const { return width; }

   unsigned getHeight() const { return height; }

   uint8_t* getStorage(unsigned& pitch_) const
   {
      pitch_ = pitch;
      return buffer;
   }

   void refresh() const
   {
      (void)write(refresh_fd, "2", 1);
   }
};


Frame::Frame(const char* /* title */, unsigned width_, unsigned height_, uint32_t /* flags */)
{
   pimpl  = new Impl();
   buffer = pimpl->getStorage(pitch);

   resize(width_, height_);
}

Frame::~Frame() { delete pimpl; }

void Frame::resize(unsigned width_, unsigned height_)
{
   width  = std::min(width_,  pimpl->getWidth());
   height = std::min(height_, pimpl->getHeight());
}

void Frame::refresh() { pimpl->refresh(); }

// Some stubs for unimplemented functionality

void* Frame::getHandle() const { return nullptr; }

uint32_t Frame::getId() const { return 0; }

void Frame::setTitle(const char* title_) {}

void Frame::setVisible(bool visible_) {}

void Frame::setGenerator(Generator* generator_) {}

} // namespace PLT
