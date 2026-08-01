#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
extern "C" void supervisorTrap();

int main() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine
    __asm__ volatile ("csrs sie, %[x]" : : [x] "r" (1 << 9)); //enabling hardware interrupts
    __asm__ volatile ("csrs sstatus, 2"); //enabling sie
    __asm__ volatile ("ecall");
    while (1);
    return 0;
}