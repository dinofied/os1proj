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

    using Body = void (*)();

    static TCB* running;
    static TCB* createThread(Body body, uint64 timeSlice);

    static void yield();

    bool isFinished();
    void setFinished(bool finished);

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

private:
    explicit TCB(Body body, uint64 timeSlice) :
        body(body),
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        context({stack != 0 ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0,
                body != 0 ? (uint64) body : 0}),
        finished(false),
        timeSlice(timeSlice)
    {
        if (body != nullptr) {Scheduler::putThread(this);}
    }

    Body body;
    uint64* stack;
    Context context;
    bool finished;
    uint64 timeSlice;

    static uint64 timeSliceCounter;

    static void dispatch();

};


#endif //PROJECT_BASE_TCB_HPP
