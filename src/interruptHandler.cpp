//
// Created by os on 7/29/26.
//
#ifndef PROJECT_BASE_INTERRUPT_HANDLER
#define PROJECT_BASE_INTERRUPT_HANDLER

#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/ajmoPrintati.hpp"
#include "../h/tcb.hpp"
#include "../h/semaphore.hpp"
#include "../h/scheduler.hpp"
#include "../h/console.hpp"
#include "../h/newdelete.hpp"

extern "C" void supervisorTrap();

extern "C" void handleSupervisorTrap() {
    volatile uint64 arg1, arg2, arg3, arg4;
    volatile uint64 scause;
    volatile uint64 sepc;
    volatile uint64 opCode;
    volatile uint64 sstatus;
    volatile uint64 stval;
    __asm__ volatile ("mv %0, a0" : "=r" (opCode));
    __asm__ volatile("mv %0, a1" : "=r" (arg1));
    __asm__ volatile("mv %0, a2" : "=r" (arg2));
    __asm__ volatile("mv %0, a3" : "=r" (arg3));
    __asm__ volatile("mv %0, a4" : "=r" (arg4));
    __asm__ volatile ("csrr %0, scause" : "=r" (scause));
    __asm__ volatile ("csrr %0, sepc" : "=r" (sepc));
    __asm__ volatile ("csrr %0, sstatus" : "=r" (sstatus));
    __asm__ volatile ("csrr %0, stval" : "=r" (stval));

    volatile uint64 ret;


    switch (scause) {
        case 0x8000000000000001: {
            //timer
            Scheduler::reduceSleepingTime();
            TCB::timeSliceCounter++;
            if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
            }
            __asm__ volatile ("csrc sip, 2"); //enabling other interrupts
            break;
        }
        case 0x8000000000000009: {
            //hardware
            int plic = plic_claim();

            //if (!Buffer::inputSem->getItems())Buffer::inputSem->signal();
            if (!Buffer::outputSem->getItems())Buffer::outputSem->signal();

            plic_complete(plic);
            //console_handler();
            uint64 seip_mask = 1 << 9;
            __asm__ volatile ("csrc sip, %0" : : "r" (seip_mask)); //enabling other interrupts
            break;
        }
        case 0x0000000000000002:
            //illegal instruction
            printajStringBolan("0x02 Illegal instruction:");
            printajBrojBolan(sepc);
            __putc('\n');
            break;
        case 0x0000000000000005:
            //unauthorized memory read
            printajStringBolan("0x05 Unauthorized mem read:");
            printajBrojBolan(sepc);
            __putc('\n');
            break;
        case 0x0000000000000007:
            //unauthorized memory write
            printajStringBolan("0x07 Unauthorized mem write:");
            printajBrojBolan(sepc);
            printajStringBolan("Address:");
            printajBrojBolan(stval);
            __putc('\n');
            break;
        case 0x0000000000000008:
        case 0x0000000000000009:


            switch (opCode) {
                //mem_alloc
                case 0x01: {
                    ret = (uint64)MemoryAllocator::getInstance().mem_alloc((size_t)arg1);
                    break;
                }
                //mem_free
                case 0x02: {
                    ret = MemoryAllocator::getInstance().mem_free((void*)arg1);
                    break;
                }
                //thread_create
                case 0x11: {
                    TCB* newThread = TCB::createThread((void(*)(void*))arg2, (void*)arg3, (uint64*)arg4);
                    newThread == nullptr ? ret = -1 : ret = 0;
                    *(TCB**)arg1 = newThread;
                    break;
                }
                //thread_exit
                case 0x12: {
                    TCB::timeSliceCounter = 0;
                    TCB::running->setFinished(true);
                    TCB* old = TCB::running;
                    TCB::dispatch();
                    delete old; //?
                    ret = 0;
                    break;
                }
                //thread_dispatch
                case 0x13: {
                    TCB::timeSliceCounter = 0;
                    TCB::dispatch();
                    break;
                }
                //sem_open
                case 0x21: {
                    Semaphore* newSem = Semaphore::createSemaphore(arg2);
                    newSem == nullptr ? ret = -1 : ret = 0;
                    *(Semaphore**)arg1 = newSem;
                    break;
                }
                //sem_close
                case 0x22: {
                    Semaphore* sem = (Semaphore*)arg1;
                    ret = sem->close();
                    break;
                }
                //sem_wait
                case 0x23: {
                    Semaphore* sem = (Semaphore*)arg1;
                    ret = sem->wait();
                    break;
                }
                //sem_signal
                case 0x24: {
                    Semaphore* sem = (Semaphore*)arg1;
                    ret = sem->signal();
                    break;
                }
                //sem_wait_n
                case 0x25: {
                    Semaphore* sem = (Semaphore*)arg1;
                    ret = sem->wait(arg2);
                    break;
                }
                //sem_signal_n
                case 0x26: {
                    Semaphore* sem = (Semaphore*)arg1;
                    ret = sem->signal(arg2);
                    break;
                }
                //thread sleep
                case 0x31: {
                    ret = Scheduler::putToSleep(arg1);
                    break;
                }
                //get_c
                case 0x41: {
                    ret = Buffer::inputBuffer->get();
                    break;
                }
                //put_c
                case 0x42: {
                    Buffer::outputBuffer->put((char)arg1);
                    break;
                }
                //init buffers
                case 0x43: {
                    Buffer::init();
                    break;
                }
            }

            sepc += 4;
            break;
    }
    __asm__ volatile ("csrw sepc, %0" : : "r" (sepc));
    __asm__ volatile("csrw sstatus, %0" : : "r" (sstatus));
    __asm__ volatile("mv a0, %0" : : "r" ((uint64)ret));

};

#endif