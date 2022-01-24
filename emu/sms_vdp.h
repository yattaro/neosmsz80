#ifndef SMS_VDP_H
#define SMS_VDP_H
#include <z80.h>


class sms_vdp
{
public:
    sms_vdp();

private:
    byte vram[0x4000];
    byte cram[0x20];
};

#endif // SMS_VDP_H
