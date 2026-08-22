//
// Created by os on 8/21/26.
//

#ifndef PROJECT_BASE_CONSOLE_HPP
#define PROJECT_BASE_CONSOLE_HPP

#include "../lib/hw.h"


class Semaphore;
typedef Semaphore* _sem;

static const uint64 BUFF_SIZE = 1024;

class Buffer {
public:

    Buffer();

    void put(char c);
    char get();

    static void inputWorker();
    static void outputWorker();

    static _sem outputSem;
    static _sem inputSem;

    static Buffer* inputBuffer;
    static Buffer* outputBuffer;

private:
    uint64 head, tail;
    char items[BUFF_SIZE];

    _sem itemsAvailable;
    _sem spaceAvailable;

};

#endif //PROJECT_BASE_CONSOLE_HPP
