#include "smsz80_common.h"
#include "config.h"
#ifndef Z80
#define Z80

union reg{
    WORD complete;
    #ifdef BIG_ENDIAN
    struct{
        BYTE high;
        BYTE low;
    };
    #else
    struct{
        BYTE low;
        BYTE high;
    };
    #endif
};

extern struct z80_regs
{
    WORD PC; // Program Counter
    WORD SP; // Stack Pointer
    WORD RR; // Refresh Register
    WORD IR; // Interrupt Register
    union reg af;
    union reg bc;
    union reg de;
    union reg hl;
    union reg afshadow;
    union reg bcshadow;
    union reg deshadow;
    union reg hlshadow;
    union reg ix;
    union reg iy;
} registers;

int next_instruction();
void inc_rr();
int exec(BYTE opcode);

#endif