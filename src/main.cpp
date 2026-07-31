#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
extern "C" void supervisorTrap();

int main() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap));
    __asm__ volatile ("ecall");
    return 0;
}