#include "sms_vdp.h"

sms_vdp::sms_vdp()
{
    vcounter = 255;
    is_second_control_write = false;
}


/*
 * Returns value of vertical counter
 */
BYTE sms_vdp::read_vcounter()
{
    return vcounter;
}

/*
 * Returns value of horizontal counter
 */
BYTE sms_vdp::read_hcounter()
{
    return hcounter;
}

void sms_vdp::write_control_word(BYTE data)
{
    (void)data;
}
