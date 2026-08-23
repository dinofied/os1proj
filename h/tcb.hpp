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

    static void setRunning(TCB* newRunning);

    struct Context {
        uint64 sp;
        uint64 ra;
        uint64 s0;
        uint64 s1;
        uint64 s2;
        uint64 s3;
        uint64 s4;
        uint64 s5;
        uint64 s6;
        uint64 s7;
        uint64 s8;
        uint64 s9;
        uint64 s10;
        uint64 s11;

    };

    Context context;

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
        stack(stack_location == nullptr ? new uint64[DEFAULT_STACK_SIZE] : stack_location),
        arg(arg),
        finished(false),
        timeSlice(timeSlice)
    {
        context.sp = (stack != 0 ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0);
        context.ra = (uint64) &threadWrapper;
        if (body != nullptr) {Scheduler::putThread(this);}
    }

    Body body;
    uint64* stack;
    void* arg;
    bool finished;
    uint64 timeSlice;

    static void threadWrapper();

};


#endif //PROJECT_BASE_TCB_HPP
