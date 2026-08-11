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
    static TCB* createThread(Body body);

    static void yield();

    bool isFinished();
    void setFinished(bool finished);

    struct Context {
        uint64 sp;
        uint64 ra;
    };

private:
    explicit TCB(Body body) :
        body(body),
        stack(body != nullptr ? new uint64[DEFAULT_STACK_SIZE] : nullptr),
        context({stack != 0 ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0,
                body != 0 ? (uint64) body : 0}),
        finished(false)
    {
        if (body != nullptr) {Scheduler::putThread(this);}
    }

    Body body;
    uint64* stack;
    Context context;
    bool finished;

    static void dispatch();

};


#endif //PROJECT_BASE_TCB_HPP
