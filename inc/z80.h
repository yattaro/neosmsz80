#include "smsz80_common.h"
#ifndef Z80
#define Z80

#ifdef BIG_ENDIAN
union reg{
    WORD complete;
    struct{
        BYTE high;
        BYTE low;
    };
};
#else
union reg{
    WORD complete;
    struct{
        BYTE low;
        BYTE high;
    };
};
#endif

extern union reg reg_af;
extern union reg reg_bc;
extern union reg reg_de;
extern union reg reg_hl;

int next_instruction();

#endif