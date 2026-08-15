//
// Created by os on 8/15/26.
//

#include "../h/semaphore.hpp"

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext); // from contextSwitch.S

Semaphore* Semaphore::createSemaphore(uint64 tokenCount) {
    Semaphore* newSem = new Semaphore(tokenCount);
    return newSem;
};

int Semaphore::signal(uint64 tokenCount) {
    remainingTokens += tokenCount;
    if (tokenCount > 0) unblockThread();
    return 0;
};

int Semaphore::wait(uint64 tokenCount) {
    if (isClosed) return -1;
    while (!isClosed && remainingTokens < tokenCount) {
        blockCurrentThread();
    }
    //if thread wakes up by close()
    if (isClosed) return -1;

    remainingTokens -= tokenCount;
    if (semWaitingThreads.peekFirst() && remainingTokens > 0) unblockThread();
    return 0;
};

int Semaphore::close() {
    isClosed = true;
    while (semWaitingThreads.peekFirst()) {unblockThread();};
    return 0;
};

void Semaphore::blockCurrentThread() {
    TCB* old = TCB::running;
    if (!old->isFinished()) {semWaitingThreads.addLast(old);}
    TCB::running = Scheduler::getNextThread();
    contextSwitch(&old->TCB::context, &TCB::running->context);
};
void Semaphore::unblockThread() {
    TCB* blocked = semWaitingThreads.removeFirst();
    if (blocked != 0) Scheduler::putThread(blocked);
};
