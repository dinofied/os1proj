//
// Created by os on 8/11/26.
//

#include "../h/scheduler.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/tcb.hpp"

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext); // from contextSwitch.S

List<TCB> Scheduler::waitingThreadQueue;

uint64 Scheduler::waitingThreadCount = 0;

Scheduler::sleepingThread* Scheduler::head = nullptr;

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
}

int Scheduler::putToSleep(uint64 sleepTime) {

    if (sleepTime <= 0) return - 1;
    insertIntoList(TCB::running, sleepTime);

    TCB* old = TCB::running;
    TCB::running = Scheduler::getNextThread();
    contextSwitch(&old->context, &TCB::running->context);

    return 0;
}

void Scheduler::insertIntoList(TCB *tcb, uint64 sleepTime) {
    // sleepingThread* toSleep = (sleepingThread*)MemoryAllocator::getInstance().
    // mem_alloc(sizeof(sleepingThread) / MEM_BLOCK_SIZE + (sizeof(sleepingThread) / MEM_BLOCK_SIZE) ? 1 : 0);
    //
    // toSleep->tcb = tcb;
    // toSleep->remainingSleep = sleepTime;
    sleepingThread* toSleep = new sleepingThread(tcb, sleepTime);
    if (!head) {
        head = toSleep;
        return;
    }

    if (toSleep->remainingSleep < head->remainingSleep) {
        toSleep->next = head;
        head->remainingSleep -= toSleep->remainingSleep;
        head = toSleep;
        return;
    }

    sleepingThread* curr = head;

    while (curr) {
        toSleep->remainingSleep -= curr->remainingSleep;

        if (!curr->next || curr->next->remainingSleep > toSleep->remainingSleep) break;
        curr = curr->next;
    }

    if (curr->next) {
        toSleep->next = curr->next;
        toSleep->next->remainingSleep -= toSleep->remainingSleep;
    }

    curr->next = toSleep;
}

void Scheduler::reduceSleepingTime() {
    if (!head) return;

    head->remainingSleep--;
    while (head && head->remainingSleep == 0) {
        Scheduler::putThread(head->tcb);
        sleepingThread* toDel = head;
        head = head->next;
        //MemoryAllocator::getInstance().mem_free(toDel);
        delete toDel;
    }
}
