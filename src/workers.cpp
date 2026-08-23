//
// Created by os on 8/11/26.
//

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/ajmoPrintati.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"

void workerIterate(void* arg) {
    char c = *(char*)arg;
    while (1) {
        putc(c);
        //__putc('\n');
    }
}

void workerBodyA(void* arg)
{
    //sem_t sem = (sem_t)arg;

    for (uint64 i = 0; i < 10; i++)
    {
        //if (sem) sem_wait_n(sem, 1);
        printajStringBolan("A: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
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
        printajStringBolan("B: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
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
        printajStringBolan("BB: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
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
        printajStringBolan("C: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
    }

    printajStringBolan("C: yield\n");
    __asm__ ("li t1, 7");
    TCB::yield();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printajStringBolan("C: t1=");
    printajBrojBolan(t1);
    printajStringBolan("\n");

    uint64 result = fibonacci(12);
    printajStringBolan("C: fibonaci=");
    printajBrojBolan(result);
    printajStringBolan("\n");

    for (; i < 6; i++)
    {
        printajStringBolan("C: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
    }
//    TCB::yield();
}

void workerBodyD(void* arg)
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        printajStringBolan("D: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
    }

    printajStringBolan("D: yield\n");
    __asm__ ("li t1, 5");
    TCB::yield();

    uint64 result = fibonacci(16);
    printajStringBolan("D: fibonaci=");
    printajBrojBolan(result);
    printajStringBolan("\n");

    for (; i < 16; i++)
    {
        printajStringBolan("D: i=");
        printajBrojBolan(i);
        printajStringBolan("\n");
    }
//    TCB::yield();

}