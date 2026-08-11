//
// Created by os on 7/29/26.
//
#ifndef PROJECT_BASE_INTERRUPT_HANDLER
#define PROJECT_BASE_INTERRUPT_HANDLER

#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/ajmoPrintati.hpp"
#include "../h/tcb.hpp"

extern "C" void supervisorTrap();

uint64 timer = 0;

extern "C" void handleSupervisorTrap() {
    volatile uint64 arg1, arg2;
    volatile uint64 scause;
    volatile uint64 sepc;
    volatile uint64 opCode;
    volatile uint64 sstatus;
    __asm__ volatile("mv %0, a1" : "=r" (arg1));
    __asm__ volatile("mv %0, a2" : "=r" (arg2));
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepc));
    __asm__ volatile ("mv %0, a0" : "=r" (opCode));
    __asm__ volatile ("csrr %0, sstatus" : "=r" (sstatus));


    volatile uint64 ret;

    // printajStringBolan("Scause, opCode:");
    // printajBrojBolan(scause);
    // printajBrojBolan(opCode);

    switch (scause) {
        case 0x8000000000000001:
            //timer
            __asm__ volatile ("csrc sip, 2"); //enabling other interrupts
            timer++;
            if (timer % 10 == 0) {
                printajStringBolan("Timer:");
                printajBrojBolan(timer/10);
                __putc('\n');
            }








            break;
        case 0x8000000000000009:
            //hardware
            console_handler();
            break;
        case 0x0000000000000002:
            //illegal instruction
            printajBrojBolan(scause);
            break;
        case 0x0000000000000005:
            //unauthorized memory read
            printajBrojBolan(scause);
            break;
        case 0x0000000000000007:
            //unauthorized memory write
            printajBrojBolan(scause);
            break;
        case 0x0000000000000008:
        case 0x0000000000000009:


            switch (opCode) {
                //mem_alloc
                case 0x01: {
                    printajStringBolan("a1");
                    printajBrojBolan(arg1);
                    ret = (uint64)MemoryAllocator::getInstance().mem_alloc((size_t)arg1);
                    printajBrojBolan(ret);
                    __putc('\n');
                    break;
                }
                //mem_free
                case 0x02: {
                    printajStringBolan("a1");
                    printajBrojBolan(arg1);
                    __putc('\n');
                    ret = MemoryAllocator::getInstance().mem_free((void*)arg1);
                    printajBrojBolan(ret);
                    __putc('\n');
                    break;
                }
            }


            sepc += 4;
            __asm__ volatile ("csrw sepc, %0" : : "r" (sepc));
            break;
    }
    __asm__ volatile("mv a0, %0" : : "r" ((uint64)ret));

};

#endif