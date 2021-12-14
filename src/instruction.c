#include "instruction.h"
#include "mem.h"
#include "smsz80_common.h"
#include "z80.h"
#include <stdbool.h>

/*
 * Load 16 bit immediate value to the given register
 * dst - pointer to destination register
 */
void ld_16bit_reg_IMM(union reg *dst)
{
    dst->low = read_mem(registers.PC);
    registers.PC++;
    dst->high = read_mem(registers.PC);
    registers.PC++;
}

/*
 * Increments the given 8 bit register and sets the appropriate flags
 * dst - pointer to register
 */
void inc_8bit(BYTE *dst)
{
    BYTE orig = *dst;
    *dst++;
    set_flags_8bit(*dst, orig, 1, false);
}

void dec_8bit(BYTE *dst)
{
    BYTE orig = *dst;
    *dst--;
    set_flags_8bit(*dst, orig, 1, true);
}

/*
 * Helper function to set flags for 8 bit arithmetic operations
 */
void set_flags_8bit(BYTE val, BYTE orig, BYTE arg, bool subtract)
{
    // XF, YF, and sign flags are copies of bits 3, 5, and 7 respectively
    flag_assign(val & (FLAG_XF | FLAG_YF | FLAG_SIGN));

    if(val == 0x0) flag_set(FLAG_ZERO);
    else flag_clear(FLAG_ZERO);

    if((orig & FLAG_SIGN) && ((orig & FLAG_SIGN) == (arg & FLAG_SIGN)))
    {
        if(!(val & FLAG_SIGN)) flag_set(FLAG_PARITY);
    }
    else if(!(orig & FLAG_SIGN) && ((orig & FLAG_SIGN) == (arg & FLAG_SIGN)))
    {
        if(val & FLAG_SIGN) flag_set(FLAG_PARITY);
    }
    else flag_clear(FLAG_PARITY);

    // How the heck does the half-carry flag work???  The world my never know
    if(!subtract)
    {
        flag_clear(FLAG_SUBTRACT);
        if(val < orig) flag_set(FLAG_CARRY);
        else if(val < arg) flag_set(FLAG_CARRY);
        else flag_clear(FLAG_CARRY);
        if((((orig & 0xf) + (arg & 0xf)) & 0x10) == 0x10)
            flag_set(FLAG_HALF_CARRY);
        else flag_clear(FLAG_HALF_CARRY);
    }
    else
    {
        flag_set(FLAG_SUBTRACT);
        if(val > orig) flag_set(FLAG_CARRY);
        else if(val > arg) flag_set(FLAG_CARRY);
        else flag_clear(FLAG_CARRY);
        if((((orig & 0xf) - (arg & 0xf)) & 0x10) == 0x10)
            flag_set(FLAG_HALF_CARRY);
        else flag_clear(FLAG_HALF_CARRY);
    }
    
}