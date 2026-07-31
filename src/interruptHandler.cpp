//
// Created by os on 7/29/26.
//
#ifndef PROJECT_BASE_INTERRUPT_HANDLER
#define PROJECT_BASE_INTERRUPT_HANDLER

#include "../lib/console.h"
#include "../lib/hw.h"

extern "C" void supervisorTrap();

extern "C" void handleSupervisorTrap() {
    __putc('T');

    uint64 sepc;
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepc));
    sepc += 4;
    __asm__ volatile ("csrw sepc, %0" : : "r" (sepc));
};

#endif