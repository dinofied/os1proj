//
// Created by os on 7/31/26.
//
#include "../h/ajmoPrintati.hpp"
#include "../h/tcb.hpp"

void printajBrojBolan(uint64 number) {
    // uint64 volatile sstatus;
    // __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    // __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    char arr[64];

    int id = 0;

    for (int j = 0; j < 64; j++) arr[j] = 0;

    if (number == 0) {arr[id++] = '0';}

    while (number > 0) {
        int num = number % 10;
        arr[id++] = '0' + num;
        number /= 10;
    }

    while (--id >= 0) __putc(arr[id]);

    //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
}


void printajStringBolan(const char *string) {
    // uint64 volatile sstatus;
    // __asm__ volatile ("csrr %0, sstatus" : "=r" (sstatus));
    // __asm__ volatile ("csrc sstatus, %0" :: "r" (TCB::SSTATUS_SIE));

    while (*string != '\0') {
        __putc(*string);
        string++;
    }

    //__asm__ volatile ("csrw sstatus, %0" :: "r" (sstatus));
}
