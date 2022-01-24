#include "z80mem.h"
#include <iomanip>
#include <stdlib.h>
#include <sstream>

z80mem::z80mem(std::string rom_file)
{
    init(new sms_rom(rom_file), false);
}

z80mem::z80mem(sms_rom *rom)
{
    init(rom, true);
}

void z80mem::init(sms_rom *rom, const bool rom_external)
{
    this->rom = rom;
    this->rom_external = rom_external;
    this->slot_0 = this->rom->pointerAt(0*ROM_PAGE_SIZE);
    this->slot_1 = this->rom->pointerAt(1*ROM_PAGE_SIZE);
    this->slot_2 = this->rom->pointerAt(2*ROM_PAGE_SIZE);
    this->main_mem = new BYTE[MEM_SIZE];
    this->bank_0 = new BYTE[ROM_PAGE_SIZE];
    this->bank_1 = new BYTE[ROM_PAGE_SIZE];
}

z80mem::~z80mem()
{
    if(!rom_external) delete rom;
    delete main_mem;
    delete bank_0;
    delete bank_1;
}

std::string z80mem::strmem()
{
    std::stringstream memss;
    slotstr(&memss, slot_0, ROM_PAGE_SIZE, 0x0);
    slotstr(&memss, slot_1, ROM_PAGE_SIZE, 0x0);
    slotstr(&memss, slot_2, ROM_PAGE_SIZE, 0x0);
    slotstr(&memss, main_mem, 2*RAM_SIZE, RAM_OFFSET);
    memss << std::endl;
    return memss.str();
}

void z80mem::slotstr(std::stringstream *memss, BYTE *slot, int slot_size, int start_offset)
{
    for(auto i = start_offset; i < slot_size; i += 0x10)
    {
        *memss << std::hex << std::setfill('0') << std::setw(4) << i << " ";
        for(auto j = 0x0; j < 0x10; j++)
        {
            *memss << std::hex << std::setfill('0') << std::setw(2) << (uint)slot[i+j];
        }
        *memss << "\n";
    }
}

/*
 * Determines if the memory segment indicated by addr is writable and stores the
 * value of data if it is, and simply returns if not (given this should be
 * undefined behavior on the actual hardware)
 */
void z80mem::write_mem(size_t param, const WORD addr, const BYTE data)
{
    (void)param;
    if(rom->isCodemasters() && (addr == 0x0 || addr == 0x4000 || addr == 0x8000))
    {
        // codemasters mapping
        mem_page_codemasters(addr, data);
    }
    // slot 0 and slot 1 are never writable
    if(addr < ROM_S2_OFFSET)
    {
        return;
    }
    // slot 2 is writable if RAM is mapped to it
    if(addr < RAM_OFFSET)
    {
        if(main_mem[0xFFFC] & 0x8)
        {
            slot_2[addr-ROM_S2_OFFSET] = data;
        }
        return;
    }
    // writing to RAM, should be fine
    main_mem[addr] = data;
    // Writing to control registers, do standard paging if non-codemasters ROM
    if(addr >= 0xFFFC && !rom->isCodemasters())
    {
        mem_page(addr, data);
    }
    // RAM Mirror
    if(addr < 0xDFFC) main_mem[addr+RAM_SIZE] = data;
    if(addr >= RAM_MIRROR_OFFSET) main_mem[addr-RAM_SIZE] = data;
}

/*
 * Returns value of memory stored at given address
 */
BYTE z80mem::read_mem(size_t param, const WORD addr)
{
    (void)param;
    // Avoid reading the control registers by using the non-mirrored addresses
    if(addr >= 0xFFFC) return main_mem[addr-0x2000];
    // Reading from lower memory
    if(addr < RAM_OFFSET)
    {
        // Memory below 0x400 in non-codemasters cartridges is always
        // mapped to the first page of ROM
        if(!rom->isCodemasters() && addr < 0x400) return rom->read(addr);

        // Slot 0
        if(addr < ROM_S1_OFFSET) return slot_0[addr];
        // Slot 1
        if(addr < ROM_S1_OFFSET) return slot_1[addr - ROM_PAGE_SIZE];
        // Slot 2
        return slot_2[addr - (2*ROM_PAGE_SIZE)];
    }

    return main_mem[addr];
}

//#define Z80_IO_OUTPUT

void z80mem::io_write(size_t param, const WORD addr, const BYTE data)
{
    (void)param;
    #ifdef Z80_IO_OUTPUT
    printf("PW %04x %02x\n", addr, data);
    #else
    (void)addr;
    (void)data;
    #endif
}

BYTE z80mem::io_read(size_t param, const WORD addr)
{
    (void)param;
    BYTE data = addr >> 8;
    #ifdef Z80_IO_OUTPUT
    printf("PR %04x %02x\n", addr, data);
    #endif
    return data;
}

/*
 * Handles standard memory paging
 */
void z80mem::mem_page(const WORD addr, const BYTE data)
{
    // Mask to first six bits if cartridge has 1MB ROM, first five if not
    BYTE page = rom->isOneMeg() ? data & 0x3F : data & 0x1F;

    switch (addr)
    {
        // Set mapping of RAM to slot 2
        case 0xFFFC:
            switch (data & 0xC)
            {
                case 0x8:
                    slot_2 = bank_0;
                    break;
                case 0xC:
                    slot_2 = bank_1;
                    break;
                default:
                break;
            }
            break;

        // Slot 0
        case 0xFFFD:
            slot_0 = rom->pointerAt(ROM_PAGE_SIZE * page);
            break;

        // Slot 1
        case 0xFFFE:
            slot_1 = rom->pointerAt(ROM_PAGE_SIZE * page);
            break;

        // Mapping ROM to slot 2
        case 0xFFFF:
            if(!(main_mem[0xFFFC] & 0x8))
            {
                slot_2 = rom->pointerAt(ROM_PAGE_SIZE * page);
            }
            break;
        default:
            break;
    }
}

/*
 * Handles memory paging for CodeMasters cartridges
 */
void z80mem::mem_page_codemasters(const WORD addr, const BYTE data)
{
    // Mask to first six bits if cartridge has 1MB ROM, first five if not
    BYTE page = rom->isOneMeg() ? data & 0x3F : data & 0x1F;
    switch (addr)
    {
        case 0x0:
            slot_0 = rom->pointerAt(ROM_PAGE_SIZE * page);
            break;
        case 0x4000:
            slot_1 = rom->pointerAt(ROM_PAGE_SIZE * page);
            break;
        case 0x8000:
            slot_2 = rom->pointerAt(ROM_PAGE_SIZE * page);
            break;
        default:
            break;
    }
}
