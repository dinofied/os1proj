//
// Created by os on 8/11/26.
//

#include "../h/scheduler.hpp"

#include "../h/ajmoPrintati.hpp"

List<TCB> Scheduler::waitingThreadQueue;

uint64 Scheduler::waitingThreadCount = 0;

TCB *Scheduler::getNextThread() {
    waitingThreadCount--;
    // printajStringBolan("Thread izbacen. Broj threadova u cekanju:");
    // printajBrojBolan((uint64)waitingThreadCount);
    // __putc('\n');
    return waitingThreadQueue.removeFirst();
}

void Scheduler::putThread(TCB *tcb) {
    waitingThreadCount++;
    // printajStringBolan("Thread dodat. Broj threadova u cekanju:");
    // printajBrojBolan((uint64)waitingThreadCount);
    // __putc('\n');
    waitingThreadQueue.addLast(tcb);
}

uint64 Scheduler::getWaitingThreadCount() {
    return waitingThreadCount;
};
