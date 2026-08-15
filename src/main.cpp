#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/ajmoPrintati.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/syscall_c.hpp"
#include "../h/semaphore.hpp"

extern "C" void supervisorTrap();

int myMainTestThread() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    thread_t* threads[5];
    for (int i = 0; i < 5; i++) threads[i] = new thread_t;

    thread_create(threads[0], nullptr, nullptr);
    TCB::setRunning((TCB*)*threads[0]);

    int ret;

    ret = thread_create(threads[1], workerBodyA, nullptr);
    if (ret == 0) printajStringBolan("ThreadA created\n");
    else printajStringBolan("ThreaA ERROR\n");

    ret = thread_create(threads[2], workerBodyB, nullptr);
    if (ret == 0) printajStringBolan("ThreadB created\n");
    else printajStringBolan("ThreaB ERROR\n");

    ret = thread_create(threads[3], workerBodyC, nullptr);
    if (ret == 0) printajStringBolan("ThreadC created\n");
    else printajStringBolan("ThreaC ERROR\n");

    ret = thread_create(threads[4], workerBodyD, nullptr);
    if (ret == 0) printajStringBolan("ThreadD created\n");
    else printajStringBolan("ThreaD ERROR\n");

    __asm__ volatile ("csrs sstatus, 2"); //enabling sie


    while (Scheduler::getWaitingThreadCount() > 0) {TCB::yield();};

    for (auto &thread: threads)
    {
        delete thread;
    }
    printajStringBolan("Finished\n");

    return 0;
}

int myMainTestSem() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    thread_t* threads[4];
    for (int i = 0; i < 4; i++) threads[i] = new thread_t;

    thread_create(threads[0], nullptr, nullptr);
    TCB::setRunning((TCB*)*threads[0]);

    int ret;

    sem_t* sem= new sem_t;
    if (sem_open(sem, 3)) {
        printajStringBolan("Semafor otvoren.");
        __putc('\n');
    };

    ret = thread_create(threads[1], workerBodyA, (void*)(*sem));
    if (ret == 0) printajStringBolan("ThreadA created\n");
    else printajStringBolan("ThreaA ERROR\n");

    ret = thread_create(threads[2], workerBodyB, (void*)(*sem));
    if (ret == 0) printajStringBolan("ThreadB created\n");
    else printajStringBolan("ThreaB ERROR\n");

    ret = thread_create(threads[3], workerBodyBB, (void*)(*sem));
    if (ret == 0) printajStringBolan("ThreadBB created\n");
    else printajStringBolan("ThreaBB ERROR\n");

    // ret = thread_create(threads[3], workerBodyB, (void*)(*sem));
    // if (ret == 0) printajStringBolan("ThreadC created\n");
    // else printajStringBolan("ThreaC ERROR\n");


    __asm__ volatile ("csrs sstatus, 2"); //enabling sie


    while (Scheduler::getWaitingThreadCount() > 0) {TCB::yield();};

    for (auto &thread: threads)
    {
        delete thread;
    }
    printajStringBolan("Finished\n");

    return 0;
}

int main() {
    // __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine
    // thread_t* kernelThread = new thread_t;
    // thread_create(kernelThread, nullptr, nullptr);
    // __asm__ volatile ("csrs sstatus, 2"); //enabling sie
    //
    // userMain();
    //
    // delete kernelThread;
    return myMainTestSem();
}