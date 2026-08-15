//
// Created by os on 8/15/26.
//

#include "../h/semaphore.hpp"
#include "../h/ajmoPrintati.hpp"

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext); // from contextSwitch.S

Semaphore* Semaphore::createSemaphore(uint64 tokenCount) {
    Semaphore* newSem = new Semaphore(tokenCount);
    printajStringBolan("Broj tokena: ");
    printajBrojBolan(newSem->remainingTokens);
    __putc('\n');
    return newSem;
};

int Semaphore::signal(uint64 tokenCount) {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    printajStringBolan("Tokens left: ");
    printajBrojBolan(remainingTokens);
    __putc('\n');
    printajStringBolan("Tokens given: ");
    printajBrojBolan(tokenCount);
    __putc('\n');


    if (isClosed) {
        __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return -1;
    }
    remainingTokens += tokenCount;
    printajStringBolan("New Token count: ");
    printajBrojBolan(remainingTokens);
    __putc('\n');
    while (blockedThreadCount > 0 && !alignFirstThreadWithTokens(remainingTokens)) unblockThread();
    __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
    return 0;
};

int Semaphore::wait(uint64 tokenCount) {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    printajStringBolan("Tokens left: ");
    printajBrojBolan(remainingTokens);
    __putc('\n');
    printajStringBolan("Tokens asked: ");
    printajBrojBolan(tokenCount);
    __putc('\n');

    if (isClosed) {
        __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return -1;
    }
    if (remainingTokens >= tokenCount) {
        remainingTokens -= tokenCount;
        printajStringBolan("New Token count: ");
        printajBrojBolan(remainingTokens);
        __putc('\n');
        __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return 0;
    }
    blockCurrentThread(tokenCount);
    //if thread wakes up by close()
    if (isClosed) {
        __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return -1;
    }

    __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
    return 0;
};

int Semaphore::close() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    isClosed = true;
    while (semBlockedThreads.peekFirst()) {
        tkTCB* curr = semBlockedThreads.removeFirst();
        Scheduler::putThread(curr->tcb);
        delete curr;
    };
    __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
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
    printajStringBolan("Thread unblocked tokens: ");
    printajBrojBolan(remainingTokens);
    __putc('\n');
    printajStringBolan("Thread unblocked asking: ");
    printajBrojBolan(remainingTokens);
    __putc('\n');
    remainingTokens -= blocked->requiredTokens;
    printajStringBolan("After unblocked thread: ");
    printajBrojBolan(remainingTokens);
    __putc('\n');
    blockedThreadCount--;
    delete blocked;
    Scheduler::putThread(blockedTCB);
};

int Semaphore::alignFirstThreadWithTokens(uint64 tokenCount) {

    printajStringBolan("Trying to find: ");
    printajBrojBolan(tokenCount);
    __putc('\n');

    for (uint64 i = 0; i < blockedThreadCount; i++) {
        tkTCB* curr = semBlockedThreads.removeFirst();
        if (!curr) return -1;
        printajBrojBolan(curr->requiredTokens);
        __putc('\n');
        if (curr->requiredTokens <= tokenCount) {
            semBlockedThreads.addFirst(curr);
            printajStringBolan("Success...");
            __putc('\n');
            return 0;
        }
        semBlockedThreads.addLast(curr);
    }
    printajStringBolan("Fail...");
    __putc('\n');
    return -1;
}
