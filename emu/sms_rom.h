#ifndef SMS_ROM_H
#define SMS_ROM_H
#include <string>
#include <z80.h>


class sms_rom
{
public:
    sms_rom(std::string filename);
    bool isOneMeg();
    bool isCodemasters();
    uint getSize();
    byte *pointerAt(const uint addr);
    byte read(const uint addr);
private:
    uint size;
    bool onemeg = false;
    bool codemasters = false;
    byte *data;
    void check_codemasters();
    void read_file(std::string filename);
};

#endif // SMS_ROM_H
