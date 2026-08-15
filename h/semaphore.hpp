//
// Created by os on 8/15/26.
//

#ifndef PROJECT_BASE_SEMAPHORE_HPP
#define PROJECT_BASE_SEMAPHORE_HPP

#include "list.hpp"
#include "tcb.hpp"

class Semaphore {
public:

    Semaphore* createSemaphore(uint64 tokenCount);

    int signal(uint64 tokenCount = 1);
    int wait(uint64 tokenCount = 1);

    int close();

    void blockCurrentThread();
    void unblockThread();

private:
    Semaphore(uint64 tokenCount) : remainingTokens(tokenCount), isClosed(false) {};

    List<TCB> semWaitingThreads;

    uint64 remainingTokens;
    bool isClosed;

    ~Semaphore() {close();};

};

#endif //PROJECT_BASE_SEMAPHORE_HPP
