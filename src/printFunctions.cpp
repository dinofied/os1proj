//
// Created by os on 7/31/26.
//
#include "../h/printFunctions.hpp"
#include "../h/syscall_c.hpp"

void printNumber(uint64 number) {
    char arr[64];

    int id = 0;

    for (int j = 0; j < 64; j++) arr[j] = 0;

    if (number == 0) {arr[id++] = '0';}

    while (number > 0) {
        int num = number % 10;
        arr[id++] = '0' + num;
        number /= 10;
    }

    while (--id >= 0) putc(arr[id]);
}


void printString(const char *string) {
    while (*string != '\0') {
        putc(*string);
        string++;
    }
}