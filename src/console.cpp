//
// Created by os on 8/21/26.
//

#include "../h/console.hpp"
#include "../h/semaphore.hpp"

_sem Buffer::outputSem = Semaphore::createSemaphore(1);
_sem Buffer::inputSem = Semaphore::createSemaphore(1);

Buffer* Buffer::inputBuffer = new Buffer();
Buffer* Buffer::outputBuffer = new Buffer();

Buffer::Buffer() {
    head = 0;
    tail = 0;

    itemsAvailable = Semaphore::createSemaphore(0);
    spaceAvailable = Semaphore::createSemaphore(BUFF_SIZE);
}

void Buffer::put(char c) {
    spaceAvailable->wait();
    items[tail++] = c;
    tail = tail % BUFF_SIZE;
    itemsAvailable->signal();
}

char Buffer::get() {
    itemsAvailable->wait();
    char c = items[head++];
    head = head & BUFF_SIZE;
    spaceAvailable->signal();
    return c;
}

void Buffer::inputWorker() {
    while (1) {
        inputSem->wait();
        while (*((uint64*)CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {

        }

    };
}

void Buffer::outputWorker() {
    while (1) {
        outputSem->wait();
        while (*((uint64*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT) {

        }

    };
}
