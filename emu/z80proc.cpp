#include "z80proc.h"
#include "sms.h"
#include "sms_types.h"


// Wrapper functions for non-static members of z80mem to be called by libz80
z80mem *current_mem;
void write_mem_wrapper(size_t param, const WORD addr, const BYTE data)
{
    current_mem->write_mem(param, addr, data);
}
BYTE read_mem_wrapper(size_t param, const WORD addr)
{
    return current_mem->read_mem(param, addr);
}
void io_write_wrapper(size_t param, const WORD addr, const BYTE data)
{
    current_mem->io_write(param, addr, data);
}
BYTE io_read_wrapper(size_t param, const WORD addr)
{
    return current_mem->io_read(param, addr);
}

z80proc::z80proc(z80mem *mem)
{
    this->mem = mem;
    current_mem = mem;
    this->context.memWrite = &write_mem_wrapper;
    this->context.memRead = &read_mem_wrapper;
    this->context.ioWrite = &io_write_wrapper;
    this->context.ioRead = &io_read_wrapper;
}

void z80proc::strregs(char *buff)
{
    snprintf(buff, 198,
            "PC =%04X SP =%04X  R =%04X  I =%04X\n"
            "AF =%04X BC =%04X DE =%04X HL =%04X\n"
            "AF'=%04X BC'=%04X DE'=%04X HL'=%04X\n"
            "IX =%04X IY =%04X\n"
            "C = %1d  N = %1d P/V = %1d XF = %1d\n"
            "H = %1d YF = %1d   Z = %1d  S = %1d",
            context.PC, context.R1.wr.SP, context.R, context.I,
            context.R1.wr.AF, context.R1.wr.BC, context.R1.wr.DE, context.R1.wr.HL,
            context.R2.wr.AF, context.R2.wr.BC, context.R2.wr.DE, context.R2.wr.HL,
            context.R1.wr.IX, context.R1.wr.IY,
            flag_read(F_C), flag_read(F_N), flag_read(F_PV), flag_read(F_3),
            flag_read(F_H), flag_read(F_5), flag_read(F_Z), flag_read(F_S));
}

bool z80proc::flag_read(Z80Flags flag)
{
    return context.R1.br.F & flag;
}

/*
 * Executees a single instruction and returns how long it took
 */
uint z80proc::exec_instr()
{
    context.tstates = 0;
    Z80Execute(&this->context);
    return this->context.tstates;
}

void z80proc::reset()
{
    Z80RESET(&this->context);
}
