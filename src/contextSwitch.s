# 1 "src/contextSwitch.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/contextSwitch.S"
.global contextSwitch
.type contextSwitch, @function
contextSwitch:

    sd sp, 0(a0)
    sd ra, 8(a0)


    ld sp, 0(a1)
    ld ra, 8(a1)

    ret
