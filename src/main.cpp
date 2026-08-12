#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/ajmoPrintati.hpp"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
extern "C" void supervisorTrap();

void spin() {
    while (1);
}
int main() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine

    TCB* threads[5];

    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA);
    printajStringBolan("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB);
    printajStringBolan("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC);
    printajStringBolan("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD);
    printajStringBolan("ThreadD created\n");

    __asm__ volatile ("csrs sstatus, 2"); //enabling sie

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {

        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printajStringBolan("Finished\n");

    return 0;
}