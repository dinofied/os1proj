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
        printString("A: i=");
        printNumber(i);
        printString("\n");
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
        printString("B: i=");
        printNumber(i);
        printString("\n");
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
        printString("BB: i=");
        printNumber(i);
        printString("\n");
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
        printString("C: i=");
        printNumber(i);
        printString("\n");
    }

    printString("C: yield\n");
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("C: t1=");
    printNumber(t1);
    printString("\n");

    uint64 result = fibonacci(12);
    printString("C: fibonaci=");
    printNumber(result);
    printString("\n");

    for (; i < 6; i++)
    {
        printString("C: i=");
        printNumber(i);
        printString("\n");
    }
//    TCB::yield();
}

void workerBodyD(void* arg)
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        printString("D: i=");
        printNumber(i);
        printString("\n");
    }

    printString("D: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();

    uint64 result = fibonacci(16);
    printString("D: fibonaci=");
    printNumber(result);
    printString("\n");

    for (; i < 16; i++)
    {
        printString("D: i=");
        printNumber(i);
        printString("\n");
    }
//    TCB::yield();

}