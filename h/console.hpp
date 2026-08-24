//
// Created by os on 8/21/26.
//

#ifndef PROJECT_BASE_CONSOLE_HPP
#define PROJECT_BASE_CONSOLE_HPP

#include "../lib/hw.h"


class Semaphore;
typedef Semaphore* sem;

class TCB;
typedef TCB* tcb;

static const uint64 BUFF_SIZE = 1000000;

class Buffer {
public:

    Buffer();

    void put(char c);
    char get();

    static void init();

    static void inputWorker(void*);
    static void outputWorker(void*);
    static void idleWorker(void*);

    static sem outputSem;
    static sem inputSem;

    static Buffer* inputBuffer;
    static Buffer* outputBuffer;

private:
    uint64 head, tail;
    char items[BUFF_SIZE];

    sem itemsAvailable;
    sem spaceAvailable;

    static tcb inputFella;
    static tcb outputFella;
};

#endif //PROJECT_BASE_CONSOLE_HPP
