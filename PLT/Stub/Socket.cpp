//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "PLT/Socket.h"

namespace PLT {

Socket::Socket() : pimpl(nullptr) {}

Socket::~Socket() {}

bool Socket::connect(const char* hostname, unsigned port) { return false; }

bool Socket::isOpen() const { return false; }

void Socket::close() {}

bool Socket::read(void* buffer, size_t buffer_size, size_t& bytes_read) { return false; }

bool Socket::write(void* buffer, size_t bytes_to_write) { return false; }

} // namespace PLT

#endif
