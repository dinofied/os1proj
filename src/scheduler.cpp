//
// Created by os on 8/11/26.
//

#include "../h/scheduler.hpp"

List<TCB> Scheduler::waitingThreadQueue;

TCB *Scheduler::getNextThread() {
    return waitingThreadQueue.removeFirst();
}

void Scheduler::putThread(TCB *tcb) {
    waitingThreadQueue.addLast(tcb);
}
