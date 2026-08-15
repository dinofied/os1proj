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
    if (isClosed) return -1;
    remainingTokens += tokenCount;
    while (blockedThreadCount > 0 && !alignFirstThreadWithTokens(remainingTokens)) unblockThread();
    return 0;
};

int Semaphore::wait(uint64 tokenCount) {
    if (isClosed) return -1;
    if (remainingTokens >= tokenCount) {
        remainingTokens -= tokenCount;
        return 0;
    }
    blockCurrentThread(tokenCount);
    //if thread wakes up by close()
    if (isClosed) return -1;

    return 0;
};

int Semaphore::close() {
    isClosed = true;
    while (semBlockedThreads.peekFirst()) {
        tkTCB* curr = semBlockedThreads.removeFirst();
        Scheduler::putThread(curr->tcb);
        delete curr;
    };
    return 0;
};

void Semaphore::blockCurrentThread(uint64 threadTokens) {
    TCB* oldTCB = TCB::running;
    tkTCB* newTkTCB = new tkTCB(oldTCB, threadTokens);
    semBlockedThreads.addLast(newTkTCB);
    blockedThreadCount++;
    TCB::running = Scheduler::getNextThread();
    contextSwitch(&oldTCB->TCB::context, &TCB::running->context);
};
void Semaphore::unblockThread() {
    tkTCB* blocked = semBlockedThreads.removeFirst();
    if (!blocked) return;
    TCB* blockedTCB = blocked->tcb;
    remainingTokens -= blocked->requiredTokens;
    blockedThreadCount--;
    delete blocked;
    Scheduler::putThread(blockedTCB);
};

int Semaphore::alignFirstThreadWithTokens(uint64 tokenCount) {

    for (uint64 i = 0; i < blockedThreadCount; i++) {
        tkTCB* curr = semBlockedThreads.removeFirst();
        if (!curr) return -1;

        if (curr->requiredTokens <= tokenCount) {
            semBlockedThreads.addFirst(curr);
            return 0;
        }
        semBlockedThreads.addLast(curr);
    }

    return -1;
}
