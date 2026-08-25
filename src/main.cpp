#include "../h/syscall_c.hpp"
#include "../h/printFunctions.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"

extern "C" void supervisorTrap();

int myMainTestThread() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    thread_t* threads[5];
    for (int i = 0; i < 5; i++) threads[i] = new thread_t;

    thread_create(threads[0], nullptr, nullptr);
    TCB::setRunning((TCB*)*threads[0]);

    int ret;

    ret = thread_create(threads[1], workerBodyA, nullptr);
    if (ret == 0) printString("ThreadA created\n");
    else printString("ThreaA ERROR\n");

    ret = thread_create(threads[2], workerBodyB, nullptr);
    if (ret == 0) printString("ThreadB created\n");
    else printString("ThreaB ERROR\n");

    ret = thread_create(threads[3], workerBodyC, nullptr);
    if (ret == 0) printString("ThreadC created\n");
    else printString("ThreaC ERROR\n");

    ret = thread_create(threads[4], workerBodyD, nullptr);
    if (ret == 0) printString("ThreadD created\n");
    else printString("ThreaD ERROR\n");

    __asm__ volatile ("csrs sstatus, 2"); //enabling sie


    while (Scheduler::getWaitingThreadCount() > 0) {TCB::yield();};

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;
}

int myMainTestSem() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    thread_t* threads[11];
    for (int i = 0; i < 11; i++) threads[i] = new thread_t;

    thread_create(threads[0], nullptr, nullptr);
    TCB::setRunning((TCB*)*threads[0]);

    initBuffers();

    __asm__ volatile ("csrs sstatus, 2"); //enabling sie

    int ret = 0;

    sem_t* sem= new sem_t;
    if (!sem_open(sem, 3)) {
        printString("Semafor otvoren.");
        putc('\n');
    };

    ret = thread_create(threads[1], workerBodyA, nullptr);
    if (ret == 0) printString("ThreadA created\n");
    else printString("ThreaA ERROR\n");

    ret = thread_create(threads[2], workerBodyB, nullptr);
    if (ret == 0) printString("ThreadB created\n");
    else printString("ThreaB ERROR\n");

    ret = thread_create(threads[3], workerBodyBB, nullptr);
    if (ret == 0) printString("ThreadBB created\n");
    else printString("ThreaBB ERROR\n");


    char c[10];

    for (int i = 4; i < 11; i++) {
        c[i - 4] = 'A' + i - 4;
        thread_create(threads[i], workerIterate, &c[i-4]);
    }



    while (Scheduler::getWaitingThreadCount() > 0) {thread_dispatch();};

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;
}

int myMainTestSem2() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    thread_t* threads[4];
    for (int i = 0; i < 4; i++) threads[i] = new thread_t;

    thread_create(threads[0], nullptr, nullptr);
    TCB::setRunning((TCB*)*threads[0]);

    int ret;

    sem_t* sem= new sem_t;
    if (sem_open(sem, 0)) {
        printString("Semafor otvoren.");
        putc('\n');
    };

    ret = thread_create(threads[1], workerBodyA, (void*)(*sem));
    if (ret == 0) printString("ThreadA created\n");
    else printString("ThreaA ERROR\n");

    ret = thread_create(threads[2], workerBodyB, (void*)(*sem));
    if (ret == 0) printString("ThreadB created\n");
    else printString("ThreaB ERROR\n");

    ret = thread_create(threads[3], workerBodyBB, (void*)(*sem));
    if (ret == 0) printString("ThreadBB created\n");
    else printString("ThreaBB ERROR\n");


    __asm__ volatile ("csrs sstatus, 2"); //enabling sie


    while (Scheduler::getWaitingThreadCount() > 0) {TCB::yield();};

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

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