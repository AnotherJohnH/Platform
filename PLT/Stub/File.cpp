//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "PLT/File.h"

namespace PLT {

File::File(const char* path, const char* filename, const char* ext) {}

File::~File() {}

bool File::isOpen() const { return false; }

bool File::isEof() const { return true; }

const char* File::getFilename() const { return nullptr; }

bool File::openForRead() { return false; }

bool File::openForWrite() { return false; }

bool File::getChar(char& ch) { return false; }

bool File::getLine(char* buffer, size_t size) { return false; }

bool File::seek(size_t offset) { return false; }

bool File::read(void* data, size_t bytes) { return false; }

void File::vprintf(const char* format, va_list ap) {}

bool File::write(const void* data, size_t bytes) { return false; }

void File::flush() {}
   
bool File::error(const char* format, ...) { return false; }

bool File::exists(const char* path) { return false; }

size_t File::size(const char* path) { return 0; }

bool File::createDir(const char* path) { return false; }

} // namespace PLT
