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
    if(is_second_control_write)
    {
        // Write MSB
        control_word &= 0xFF;
        control_word |= data << 8;
        is_second_control_write = false;

        switch(getControlWordCodeRegister())
        {

        }
    }
}

/*
 * Returns address stored in control register used to determine where port
 * writes to 0xBE are stored
 */
WORD sms_vdp::getControlWordAddressRegister()
{
    return control_word & 0x3FFF;
}

/*
 * Returns nibble used to determine how port writes to 0xBE are executed
 */
BYTE sms_vdp::getControlWordCodeRegister()
{
    return (BYTE)(control_word >> 14);
}
