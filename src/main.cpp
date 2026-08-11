#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/ajmoPrintati.hpp"
extern "C" void supervisorTrap();

int main() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine
    __asm__ volatile ("csrs sie, %[x]" : : [x] "r" (1 << 9)); //enabling hardware interrupts (does nothing?)  ***
    __asm__ volatile ("csrs sstatus, 2"); //enabling sie



    return 0;
}