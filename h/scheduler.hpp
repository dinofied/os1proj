//
// Created by os on 8/11/26.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler {
public:

    static TCB* getNextThread();
    static void putThread(TCB* tcb);


private:
    static List<TCB> waitingThreadQueue;
};

#endif //PROJECT_BASE_SCHEDULER_HPP
