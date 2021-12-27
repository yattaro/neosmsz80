#ifndef Z80PROC_H
#define Z80PROC_H
#include <z80.h>
#include <string>
#include "z80mem.h"

class z80proc
{
public:
    z80proc(z80mem *mem);
    void strregs(char *);
    bool flag_read(Z80Flags flag);
    uint exec_instr();
    void reset();
private:
    Z80Context context;
    z80mem *mem;
};

#endif // Z80PROC_H
