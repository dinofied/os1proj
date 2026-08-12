//
// Created by os on 8/11/26.
//

#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

class TCB {
public:
    ~TCB() {delete[] stack;}

    using Body = void (*)(void*);

    static TCB* running;
    static TCB* createThread(Body body, void* arg, uint64* stack_location);

    static void yield();

    bool isFinished();
    void setFinished(bool finished);

    void* getThreadArg();

    uint64 getTimeSlice();

    struct Context {
        uint64 sp;
        uint64 ra;
    };

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_SEIP = (1 << 9)
    };

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8)
    };

    static uint64 timeSliceCounter;

    static void dispatch();

private:
    explicit TCB(Body body, void* arg, uint64* stack_location, uint64 timeSlice) :
        body(body),
        stack(stack_location == nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        context({stack != 0 ? (uint64) &stack[DEFAULT_STACK_SIZE] - 1 : 0,
                (uint64) &threadWrapper}),
        arg(arg),
        finished(false),
        timeSlice(timeSlice)
    {
        if (body != nullptr) {Scheduler::putThread(this);}
    }

    Body body;
    uint64* stack;
    Context context;
    void* arg;
    bool finished;
    uint64 timeSlice;

    static void threadWrapper();

};


#endif //PROJECT_BASE_TCB_HPP
