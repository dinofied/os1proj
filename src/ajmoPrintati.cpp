//
// Created by os on 7/31/26.
//
#include "../h/ajmoPrintati.hpp"

void printajBrojBolan(uint64 number) {
    char arr[64];

    int id = 0;

    for (int j = 0; j < 64; j++) arr[j] = 0;

    while (number > 0) {
        int num = number % 10;
        arr[id++] = '0' + num;
        number /= 10;
    }

    while (--id >= 0) __putc(arr[id]);
    __putc('\n');

}

