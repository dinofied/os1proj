#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"
#include "../h/ajmoPrintati.hpp"
extern "C" void supervisorTrap();

int main() {
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap)); //setting interrupt routine
    __asm__ volatile ("csrs sie, %[x]" : : [x] "r" (1 << 9)); //enabling hardware interrupts
    //__asm__ volatile ("csrs sstatus, 2"); //enabling sie

    void* addr1 = mem_alloc(128);
    void* addr2 = mem_alloc(130);

    int ret1 = mem_free(addr1);
    int ret2 = mem_free(addr2);


    __putc('M');
    __putc('\n');

    printajBrojBolan((uint64)addr1);
    printajBrojBolan((uint64)addr2);
    __putc('\n');

    if (!ret1) __putc('W');
    if (!ret2) __putc('W');

    return 0;
}