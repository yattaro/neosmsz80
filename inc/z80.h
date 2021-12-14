#include "smsz80_common.h"
#include "config.h"
#include <stdbool.h>
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

enum z80_flags
{
    FLAG_CARRY = 0x01,
    FLAG_SUBTRACT = 0x02,
    FLAG_PARITY = 0x04,
    FLAG_XF = 0x08,
    FLAG_HALF_CARRY = 0x10,
    FLAG_YF = 0x20,
    FLAG_ZERO = 0x40,
    FLAG_SIGN = 0x80
};

int next_instruction();
void inc_rr();
void flag_set(enum z80_flags flag);
void flag_clear(enum z80_flags flag);
void flag_assign(enum z80_flags flag);
bool flag_read(enum z80_flags flag);
int exec_instr(BYTE opcode);

#endif