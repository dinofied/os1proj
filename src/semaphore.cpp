//
// Created by os on 8/15/26.
//

#include "../h/semaphore.hpp"
#include "../h/ajmoPrintati.hpp"
#include "../h/syscall_c.hpp"
#include "../h/newdelete.hpp"
#include "../h/MemoryAllocator.hpp"

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext); // from contextSwitch.S

Semaphore* Semaphore::createSemaphore(uint64 tokenCount) {
    // Semaphore* newSem = (Semaphore*)MemoryAllocator::getInstance().
    // mem_alloc(sizeof(Semaphore) / MEM_BLOCK_SIZE + (sizeof(Semaphore) % MEM_BLOCK_SIZE ? 1 : 0));
    // newSem->remainingTokens = tokenCount;
    // newSem->isClosed = false;

    Semaphore* newSem = new Semaphore(tokenCount);
    return newSem;
};

int Semaphore::signal(uint64 tokenCount) {
    // uint64 volatile sstatus;
    // __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    // __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    if (isClosed) {
        //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return -1;
    }
    remainingTokens += tokenCount;

    while (blockedThreadCount > 0 && !alignFirstThreadWithTokens(remainingTokens)) unblockThread();
    //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
    return 0;
};

int Semaphore::wait(uint64 tokenCount) {
    // uint64 volatile sstatus;
    // __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    // __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    if (isClosed) {
        //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return -1;
    }
    if (remainingTokens >= tokenCount) {
        remainingTokens -= tokenCount;
        //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return 0;
    }
    blockCurrentThread(tokenCount);
    //if thread wakes up by close()
    if (isClosed) {
        //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
        return -1;
    }

    //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
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
        //MemoryAllocator::getInstance().mem_free(curr);
        delete curr;
    };
    __asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
    return 0;
};

void Semaphore::blockCurrentThread(uint64 threadTokens) {
    TCB* oldTCB = TCB::running;
    // tkTCB* newTkTCB = (tkTCB*)MemoryAllocator::getInstance().
    // mem_alloc(sizeof(tkTCB) / MEM_BLOCK_SIZE + (sizeof(tkTCB) % MEM_BLOCK_SIZE ? 1 : 0));
    // newTkTCB->tcb = oldTCB;
    // newTkTCB->requiredTokens = threadTokens;
    tkTCB*  newTkTCB = new tkTCB(oldTCB, threadTokens);
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
    //MemoryAllocator::getInstance().mem_free(blocked);
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

uint64 Semaphore::getItems() {
    return remainingTokens;
}
