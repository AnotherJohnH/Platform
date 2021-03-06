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
