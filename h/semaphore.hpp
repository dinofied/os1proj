//
// Created by os on 8/15/26.
//

#ifndef PROJECT_BASE_SEMAPHORE_HPP
#define PROJECT_BASE_SEMAPHORE_HPP

#include "list.hpp"
#include "tcb.hpp"

class semaphore {
public:

    semaphore* createSemaphore(uint64 tokenCount);

    int signal(uint64 tokenCount = 1);
    int wait(uint64 tokenCount = 1);

    int close();

    int blockCurrentThread();

    int getSemWaitingCount();

private:
    semaphore(uint64 tokenCount) : tokenCount(tokenCount), semWaitingCount(0), isClosed(false) {};

    List<TCB> semWaitingThreads;

    uint64 tokenCount;
    uint64 semWaitingCount;
    bool isClosed;

};

#endif //PROJECT_BASE_SEMAPHORE_HPP
