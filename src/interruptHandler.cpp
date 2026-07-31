//
// Created by os on 7/29/26.
//
#ifndef PROJECT_BASE_INTERRUPT_HANDLER
#define PROJECT_BASE_INTERRUPT_HANDLER

#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

extern "C" void supervisorTrap();
extern "C" void printajBolan(uint64 number);

extern "C" void handleSupervisorTrap() {
    uint64 scause;
    uint64 sepc;
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepc));

    switch (scause) {
        case 0x8000000000000001:
            //timer
            break;
        case 0x8000000000000009:
            //hardware
            break;
        case 0x02:
            //illegal instruction
            break;
        case 0x05:
            //unauthorized memory read
            break;
        case 0x07:
            //unauthorized memory write
            break;
        case 0x08:
            //redirect to 0x09
        case 0x09:
            printajBolan(scause);

            sepc += 4;
            __asm__ volatile ("csrw sepc, %0" : : "r" (sepc));
            break;
    }

    //ECALL: pushing pc to next instruction to prevent loop
};

#endif