//
// Created by os on 8/11/26.
//
#include "../h/tcb.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

extern "C" void pushRegisters(); // from regUtil.S
extern "C" void popRegisters(); // from regUtil.S
extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext); // from contextSwitch.S

TCB* TCB::createThread(Body body, uint64 timeSlice) {
    return new TCB(body, timeSlice);
};

void TCB::yield() {
    pushRegisters();

    dispatch();

    popRegisters();
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