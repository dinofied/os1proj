#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/printFunctions.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"

extern "C" void supervisorTrap();


int myTest() {
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
        _printString("Semafor otvoren.");
        putc('\n');
    };

    ret = thread_create(threads[1], workerBodyA, nullptr);
    if (ret == 0) _printString("ThreadA created\n");
    else _printString("ThreaA ERROR\n");

    ret = thread_create(threads[2], workerBodyB, nullptr);
    if (ret == 0) _printString("ThreadB created\n");
    else _printString("ThreaB ERROR\n");

    ret = thread_create(threads[3], workerBodyBB, nullptr);
    if (ret == 0) _printString("ThreadBB created\n");
    else _printString("ThreaBB ERROR\n");


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
    _printString("Finished\n");

    return 0;
}


extern void userMain();

void userWrapper(void*) {
    userMain();
}


int main() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    thread_t* kernelThread = new thread_t;
    thread_create(kernelThread, nullptr, nullptr);
    TCB::setRunning((TCB*)*kernelThread);

    initBuffers();

    __asm__ volatile ("csrs sstatus, 2"); //enabling sie

    thread_t* userMain = new thread_t;
    thread_create(userMain, userWrapper, nullptr);



    return 0;
}