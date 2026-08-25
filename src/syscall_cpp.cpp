//
// Created by os on 8/15/26.
//

#include "../lib/hw.h"
//#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"


//new/delete
void *operator new(size_t size) {
    return mem_alloc(size);
}

void *operator new [](size_t  size) {
    return mem_alloc(size);
}

void operator delete(void *addr) {
    mem_free(addr);
}

void operator delete[](void *addr) {
    mem_free(addr);
}


//Thread
Thread::Thread(void (*body)(void*), void* arg) {
    this->body = body;
    this->arg = arg;
    this->myHandle = nullptr;
}

Thread::~Thread() {
    thread_exit();
}

int Thread::start() {
    int ret = thread_create(&myHandle, body, arg);
    return ret;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    int ret = time_sleep(time);
    return ret;
}

Thread::Thread() {
    this->body = redirectToRun;
    this->arg = this;
    this->myHandle = nullptr;
}

void Thread::redirectToRun(void* arg) {
    ((Thread*)arg)->run();
}

//Semaphore
Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::signal() {
    int ret = sem_signal(myHandle);
    return ret;
}

int Semaphore::wait() {
    int ret = sem_wait(myHandle);
    return ret;
}

//Preiodic Thread
void PeriodicThread::terminate() {
    period = 0;
}

PeriodicThread::PeriodicThread(time_t period) : Thread(&periodicWrapper, this){
    this->period = period;
}

void PeriodicThread::periodicWrapper(void* arg) {
    while (((PeriodicThread*)arg)->period > 0) {
        ((PeriodicThread*)arg)->periodicActivation();
        time_sleep(((PeriodicThread*)arg)->period);
    }
};


//Console
char Console::getc() {
    char c = ::getc();
    return c;
}

void Console::putc(char c) {
    ::putc(c);
}
