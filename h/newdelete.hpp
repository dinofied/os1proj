//
// Created by os on 8/11/26.
//

#ifndef PROJECT_BASE_NEWDELETE_HPP
#define PROJECT_BASE_NEWDELETE_HPP

#include "../lib/hw.h"
#include "syscall_c.hpp"

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* addr);
void operator delete[](void* addr);

#endif //PROJECT_BASE_NEWDELETE_HPP
