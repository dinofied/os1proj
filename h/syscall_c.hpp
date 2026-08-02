//
// Created by os on 8/2/26.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"

uint64 ecall_wrapper (uint64 opcode, uint64 a1 = 0,uint64 a2 = 0,uint64 a3 = 0,uint64 a4 = 0);

void* mem_alloc(size_t size);
int mem_free(void* addr);


#endif //PROJECT_BASE_SYSCALL_C_HPP
