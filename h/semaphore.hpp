//
// Created by os on 8/15/26.
//

#ifndef PROJECT_BASE_SEMAPHORE_HPP
#define PROJECT_BASE_SEMAPHORE_HPP

#include "list.hpp"
#include "tcb.hpp"

class Semaphore {
public:

    static Semaphore* createSemaphore(uint64 tokenCount);

    int signal(uint64 tokenCount = 1);
    int wait(uint64 tokenCount = 1);

    int close();

    void blockCurrentThread(uint64 threadTokens);
    void unblockThread();

    uint64 getItems();

    struct tkTCB {
        TCB* tcb;
        uint64 requiredTokens;

        tkTCB(TCB* tcb, uint64 requiredTokens) : tcb(tcb), requiredTokens(requiredTokens) {};
    };

private:
    Semaphore(uint64 tokenCount) : remainingTokens(tokenCount), isClosed(false), blockedThreadCount(0) {};

    List<tkTCB> semBlockedThreads;

    int alignFirstThreadWithTokens(uint64 tokenCount);

    uint64 remainingTokens;
    bool isClosed;
    uint64 blockedThreadCount;

    ~Semaphore() {close();};

};

#endif //PROJECT_BASE_SEMAPHORE_HPP
