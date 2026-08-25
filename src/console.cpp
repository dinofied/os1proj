//
// Created by os on 8/21/26.
//

#include "../h/console.hpp"
#include "../h/semaphore.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"

sem charBuffer::outputSem = nullptr;
sem charBuffer::inputSem = nullptr;

charBuffer* charBuffer::inputBuffer = nullptr;
charBuffer* charBuffer::outputBuffer = nullptr;

tcb charBuffer::inputFella = nullptr;
tcb charBuffer::outputFella = nullptr;
tcb charBuffer::idleFella = nullptr;

void charBuffer::inputWorker(void*) {
    while (1) {
        sem_wait((sem_t)inputSem);
        while (*((uint8*)CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {
            char c = (char) *((char*)CONSOLE_RX_DATA);
            inputBuffer->put(c);
        }

    };
}

void charBuffer::outputWorker(void*) {
    while (1) {
        sem_wait((sem_t)outputSem);
        while (*((uint8*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT) {
            char c = outputBuffer->get();
            *((char*)CONSOLE_TX_DATA) = c;
        }

    };
}

void charBuffer::idleWorker(void *) {
    while (1) {thread_dispatch();}
}

void charBuffer::init() {
    outputSem = Semaphore::createSemaphore(1);
    inputSem = Semaphore::createSemaphore(1);

    inputBuffer = new charBuffer();
    outputBuffer = new charBuffer();

    inputFella = TCB::createThread((void(*)(void*))inputWorker, nullptr, nullptr);
    outputFella = TCB::createThread((void(*)(void*))outputWorker, nullptr, nullptr);
    idleFella = TCB::createThread((void(*)(void*))idleWorker, nullptr, nullptr);
}

charBuffer::charBuffer() {
    head = 0;
    tail = 0;
    items[0] = '\0';
    itemsAvailable = Semaphore::createSemaphore(0);
    spaceAvailable = Semaphore::createSemaphore(BUFF_SIZE);
}

void charBuffer::put(char c) {
    sem_wait((sem_t)spaceAvailable);
    items[tail++] = c;
    tail = tail % BUFF_SIZE;
    sem_signal((sem_t)itemsAvailable);
}

char charBuffer::get() {
    sem_wait((sem_t)itemsAvailable);
    char c = items[head++];
    head = head % BUFF_SIZE;
    sem_signal((sem_t)spaceAvailable);
    return c;
}
