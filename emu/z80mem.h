#ifndef Z80MEM_H
#define Z80MEM_H
#include <z80.h>
#include <string>
#include "sms_rom.h"

#define ROM_PAGE_SIZE 0x4000
#define ROM_S0_OFFSET 0x0
#define ROM_S1_OFFSET 0x4000
#define ROM_S2_OFFSET 0x8000
#define RAM_SIZE 0x2000
#define RAM_OFFSET 0xC000
#define RAM_MIRROR_OFFSET 0xE000
#define MEM_SIZE 0x10000

class z80mem
{
public:
    z80mem(std::string rom_file);
    z80mem(sms_rom *rom);
    ~z80mem();
    std::string strmem();
    void write_mem(size_t param, const ushort addr, const byte data);
    byte read_mem(size_t param, const ushort addr);
    void io_write(size_t param, const ushort addr, const byte data);
    byte io_read(size_t param, const ushort addr);
private:
    void slotstr(std::stringstream *memss, byte *slot, int slot_size, int start_offset);
    void init(sms_rom *rom, const bool rom_external);
    void mem_page(const ushort addr, const byte data);
    void mem_page_codemasters(const ushort addr, const byte data);
    byte *slot_0;
    byte *slot_1;
    byte *slot_2;
    byte *main_mem;
    byte *bank_0;
    byte *bank_1;
    sms_rom *rom;
    bool rom_external;
};

#endif // Z80MEM_H
