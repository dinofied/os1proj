//
// Created by os on 7/29/26.
//
#ifndef PROJECT_BASE_INTERRUPT_HANDLER
#define PROJECT_BASE_INTERRUPT_HANDLER

#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/ajmoPrintati.hpp"

extern "C" void supervisorTrap();

uint64 timer = 0;

extern "C" void handleSupervisorTrap() {
    uint64 scause;
    uint64 sepc;
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepc));

    switch (scause) {
        case 0x8000000000000001UL:
            //timer
            timer++;
            if (timer % 10 == 0) {
                printajBolan(timer/10);
            }
            __asm__ volatile ("csrc sip, 2");
            break;
        case 0x8000000000000009:
            //hardware
            break;
        case 0x02:
            //illegal instruction
            printajBolan(scause);
            break;
        case 0x05:
            //unauthorized memory read
            printajBolan(scause);
            break;
        case 0x07:
            //unauthorized memory write
            printajBolan(scause);
            break;
        case 0x08:
            //redirect to 0x09
        case 0x09:
            printajBolan(scause);

            sepc += 4;
            __asm__ volatile ("csrw sepc, %0" : : "r" (sepc));
            break;
    }


};

#endif