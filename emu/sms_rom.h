#ifndef SMS_ROM_H
#define SMS_ROM_H
#include <string>
#include "sms_types.h"


class sms_rom
{
public:
    sms_rom(std::string filename);
    bool isOneMeg();
    bool isCodemasters();
    uint getSize();
    BYTE *pointerAt(const uint addr);
    BYTE read(const uint addr);
private:
    uint size;
    bool onemeg = false;
    bool codemasters = false;
    BYTE *data;
    void check_codemasters();
    void read_file(std::string filename);
};

#endif // SMS_ROM_H
