//
// Created by os on 8/2/26.
//

#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/ajmoPrintati.hpp"

uint64 ecall_wrapper (uint64 opcode, uint64 a1, uint64 a2,uint64 a3, uint64 a4) {
    __asm__ volatile ("ecall");
    volatile uint64 ret;
    __asm__ volatile ("mv %0, a0" : "=r" (ret));
    return ret;
}

void *mem_alloc(size_t size) {
    size = size / MEM_BLOCK_SIZE + (size % MEM_BLOCK_SIZE ? 1 : 0);
    uint64 ret = ecall_wrapper(0x01, (uint64)size);
    return (void*)ret;
}

int mem_free(void* addr) {
    uint64 ret = ecall_wrapper(0x02, (uint64)addr);
    return (int)ret;
};

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    uint64* stack_location = (uint64*)mem_alloc(DEFAULT_STACK_SIZE);
    uint64 ret = ecall_wrapper(0x11, (uint64)handle, (uint64)start_routine, (uint64)arg, (uint64)stack_location);
    return (int)ret;
};

int thread_exit() {
    uint64 ret = ecall_wrapper(0x12);
    return (int)ret;
};

void thread_dispatch() {
    ecall_wrapper(0x13);
};

int sem_open(sem_t* handle, unsigned init) {
    uint64 ret = ecall_wrapper(0x21, (uint64)handle,(uint64)init);
    return ret;
};
int sem_close(sem_t handle) {
    uint64 ret = ecall_wrapper(0x22, (uint64)handle);
    return ret;
};
int sem_wait(sem_t id) {
    uint64 ret = ecall_wrapper(0x23, (uint64)id);
    return ret;
};
int sem_signal(sem_t id) {
    uint64 ret = ecall_wrapper(0x24, (uint64)id);
    return ret;
};
int sem_wait_n(sem_t id, unsigned n) {
    uint64 ret = ecall_wrapper(0x25, (uint64)id,(uint64)n);
    return ret;
};
int sem_signal_n(sem_t id, unsigned n) {
    uint64 ret = ecall_wrapper(0x26, (uint64)id,(uint64)n);
    return ret;
};

int time_sleep(time_t time) {
    uint64 ret = ecall_wrapper(0x31, (uint64)time);
    return ret;
}

char getc() {
    uint64 ret = ecall_wrapper(0x41);
    return (char)ret;
}

void putc(char c) {
    ecall_wrapper(0x42, (uint64)c);
}

void initBuffers() {
    ecall_wrapper(0x43);
};