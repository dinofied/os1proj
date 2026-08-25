//
// Created by os on 8/11/26.
//
#include "../h/tcb.hpp"
#include "../h/MemoryAllocator.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext); // from contextSwitch.S

TCB* TCB::createThread(Body body, void* arg, uint64* stack_location) {

    TCB* newTcb = new TCB(body, arg, stack_location, DEFAULT_TIME_SLICE);

    return newTcb;
};

void TCB::yield() {
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
};

bool TCB::isFinished(){return finished;};

void TCB::setFinished(bool finished) {this->finished = finished;};

uint64 TCB::getTimeSlice() {
    return this->timeSlice;
}

void TCB::dispatch() {
    TCB* old = running;
    if (!old->isFinished()) {Scheduler::putThread(old);}
    running = Scheduler::getNextThread();
    contextSwitch(&old->context, &running->context);
};

void* TCB::getThreadArg() {
    return arg;
};

void TCB::setRunning(TCB *newRunning) {
    running = newRunning;
}

void bombo() {
    int mask = 1 << 8;
    __asm__ volatile("csrc sstatus, %0" : :"r" (mask));
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
};

void TCB::threadWrapper() {
    bombo();
    running->body(running->getThreadArg());
    running->setFinished(true);
    TCB::yield();
}

