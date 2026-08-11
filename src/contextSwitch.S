.global contextSwitch
.type contextSwitch, @function
contextSwitch:
    //saving old context to the struct
    sd sp, 0(a0)
    sd ra, 8(a0)

    //loading new context from running
    ld sp, 0(a1)
    ld ra, 8(a1)

    ret