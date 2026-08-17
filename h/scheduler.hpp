//
// Created by os on 8/11/26.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "list.hpp"
#include "../lib/hw.h"

class TCB;

class Scheduler {
public:

    static TCB* getNextThread();
    static void putThread(TCB* tcb);
    static uint64 getWaitingThreadCount();

    static void reduceSleepingTime();
    static int putToSleep(uint64 sleepTime);

private:
    struct sleepingThread {
        TCB* tcb;
        uint64 remainingSleep;

        sleepingThread* next;

        sleepingThread(TCB* tcb, uint64 sleepTime) : tcb(tcb), remainingSleep(sleepTime), next(nullptr) {};
    };

    static uint64 waitingThreadCount;
    static List<TCB> waitingThreadQueue;
    static sleepingThread* head;

    static void insertIntoList(TCB* tcb, uint64 sleepTime);

};

#endif //PROJECT_BASE_SCHEDULER_HPP
