//
// Created by os on 8/11/26.
//

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/printFunctions.hpp"
#include "../h/syscall_c.hpp"

void workerIterate(void* arg) {
    char c = *(char*)arg;
    uint64 counter = 0;
    while (1) {
        counter++;
        if (counter % 10000000 == 0) putc(c);
    }
}

void workerBodyA(void* arg)
{
    //sem_t sem = (sem_t)arg;

    for (uint64 i = 0; i < 10; i++)
    {
        //if (sem) sem_wait_n(sem, 1);
        _printString("A: i=");
        _printNumber(i);
        _printString("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
        //if (sem) sem_signal_n(sem, 1);
    }
}

void workerBodyB(void* arg)
{
    //sem_t sem = (sem_t)arg;
    for (uint64 i = 0; i < 16; i++)
    {
        //if (sem) sem_wait_n(sem, 2);
        _printString("B: i=");
        _printNumber(i);
        _printString("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
        //if (sem) sem_signal_n(sem, 2);
    }
}

void workerBodyBB(void* arg)
{
    //sem_t sem = (sem_t)arg;
    for (uint64 i = 0; i < 16; i++)
    {
        //if (sem) sem_wait_n(sem, 3);
        _printString("BB: i=");
        _printNumber(i);
        _printString("\n");
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
            //            TCB::yield();
        }
        //if (sem) sem_signal_n(sem, 3);
    }
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { TCB::yield(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void* arg)
{
    uint8 i = 0;
    for (; i < 3; i++)
    {
        _printString("C: i=");
        _printNumber(i);
        _printString("\n");
    }

    _printString("C: yield\n");
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    _printString("C: t1=");
    _printNumber(t1);
    _printString("\n");

    uint64 result = fibonacci(12);
    _printString("C: fibonaci=");
    _printNumber(result);
    _printString("\n");

    for (; i < 6; i++)
    {
        _printString("C: i=");
        _printNumber(i);
        _printString("\n");
    }
//    TCB::yield();
}

void workerBodyD(void* arg)
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        _printString("D: i=");
        _printNumber(i);
        _printString("\n");
    }

    _printString("D: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();

    uint64 result = fibonacci(16);
    _printString("D: fibonaci=");
    _printNumber(result);
    _printString("\n");

    for (; i < 16; i++)
    {
        _printString("D: i=");
        _printNumber(i);
        _printString("\n");
    }
//    TCB::yield();

}