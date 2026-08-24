//
// Created by os on 8/21/26.
//

#include "../h/console.hpp"
#include "../h/semaphore.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"

sem Buffer::outputSem = nullptr;
sem Buffer::inputSem = nullptr;

Buffer* Buffer::inputBuffer = nullptr;
Buffer* Buffer::outputBuffer = nullptr;

tcb Buffer::inputFella = nullptr;
tcb Buffer::outputFella = nullptr;

void Buffer::inputWorker(void*) {
    while (1) {
        sem_wait((sem_t)inputSem);
        while (*((uint8*)CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {
            char c = (char) *((char*)CONSOLE_RX_DATA);
            inputBuffer->put(c);
        }

    };
}

void Buffer::outputWorker(void*) {
    while (1) {
        sem_wait((sem_t)outputSem);
        while (*((uint8*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT) {
            char c = outputBuffer->get();
            *((char*)CONSOLE_TX_DATA) = c;
        }

    };
}

void Buffer::idleWorker(void *) {
    while (1) {thread_dispatch();}
}

void Buffer::init() {
    outputSem = Semaphore::createSemaphore(1);
    inputSem = Semaphore::createSemaphore(1);

    inputBuffer = new Buffer();
    outputBuffer = new Buffer();

    //inputFella = TCB::createThread((void(*)(void*))inputWorker, nullptr, nullptr);
    //outputFella = TCB::createThread((void(*)(void*))outputWorker, nullptr, nullptr);
}

Buffer::Buffer() {
    head = 0;
    tail = 0;
    items[0] = '\0';
    itemsAvailable = Semaphore::createSemaphore(0);
    spaceAvailable = Semaphore::createSemaphore(BUFF_SIZE);
}

// void Buffer::put(char c) {
//     spaceAvailable->wait();
//     items[tail++] = c;
//     tail = tail % BUFF_SIZE;
//     itemsAvailable->signal();
// }
//
// char Buffer::get() {
//     itemsAvailable->wait();
//     char c = items[head++];
//     head = head % BUFF_SIZE;
//     spaceAvailable->signal();
//     return c;
// }


void Buffer::put(char c) {
    sem_wait((sem_t)spaceAvailable);
    items[tail++] = c;
    tail = tail % BUFF_SIZE;
    sem_signal((sem_t)itemsAvailable);
}

char Buffer::get() {
    sem_wait((sem_t)itemsAvailable);
    char c = items[head++];
    head = head % BUFF_SIZE;
    sem_signal((sem_t)spaceAvailable);
    return c;
}
