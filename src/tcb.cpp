//
// Created by os on 8/11/26.
//
#include "../h/tcb.hpp"

TCB* TCB::running = nullptr;
extern "C" void pushRegisters();
extern "C" void popRegisters();
extern "C" void contextSwitch(TCB::Context* oldContext, TCB::Context* newContext);

TCB* TCB::createThread(Body body) {
    return new TCB(body);
};

void TCB::yield() {
    pushRegisters();

    dispatch();

    popRegisters();
};

bool TCB::isFinished(){return finished;};

void TCB::setFinished(bool finished) {this->finished = finished;};

void TCB::dispatch() {
    TCB* old = running;
    if (!old->isFinished()) {Scheduler::putThread(old);}
    running = Scheduler::getNextThread();
    contextSwitch(&old->context, &running->context);
};