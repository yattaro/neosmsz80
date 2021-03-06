#ifndef SMS_VDP_H
#define SMS_VDP_H
#include "sms_types.h"


class sms_vdp
{
public:
    sms_vdp();
    BYTE read_vcounter();
    BYTE read_hcounter();
    void write_control_word(BYTE data);

private:
    BYTE vram[0x4000];
    BYTE cram[0x20];
    BYTE vdp_regs[0xB];
    BYTE vcounter;
    BYTE hcounter;
    WORD control_word;
    bool is_second_control_write;
};

#endif // SMS_VDP_H
