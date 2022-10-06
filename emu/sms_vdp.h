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
    WORD getControlWordAddressRegister();
    BYTE getControlWordCodeRegister();
    BYTE vram[0x4000];
    BYTE cram[0x20];
    BYTE vdp_regs[0xB];
    BYTE vcounter;
    BYTE hcounter;
    WORD control_word;
    enum class ControlMode
    {
        write_vram_and_increment = 0,
        write_vram = 1,
        write_and_write_regs = 2,
        write_cram = 3
    };
    BYTE read_buffer;
    bool is_second_control_write;
};

#endif // SMS_VDP_H
