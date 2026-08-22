//
// Created by os on 8/2/26.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"

class _thread;
typedef _thread* thread_t;

class _sem;
typedef _sem* sem_t;

typedef unsigned long time_t;

uint64 ecall_wrapper (uint64 opcode, uint64 a1 = 0,uint64 a2 = 0,uint64 a3 = 0,uint64 a4 = 0);

void* mem_alloc(size_t size);
int mem_free(void* addr);

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_wait_n(sem_t id, unsigned n);
int sem_signal_n(sem_t id, unsigned n);

char getc();
void putc(char c);
void initBuffers();

int time_sleep(time_t time);

#endif //PROJECT_BASE_SYSCALL_C_HPP
