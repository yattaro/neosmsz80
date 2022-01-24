#include "sms_rom.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include "z80mem.h"

sms_rom::sms_rom(std::string filename)
{
    read_file(filename);
}

/*
 * Returns true if ROM is 1MB in size (important for paging)
 */
bool sms_rom::isOneMeg()
{
    return onemeg;
}

/*
 * Returns true if ROM uses Codemasters paging format.
 */
bool sms_rom::isCodemasters()
{
    return codemasters;
}

/*
 * Returns the size of the ROM.
 */
uint sms_rom::getSize()
{
    return size;
}

/*
 * Returns a pointer to the ROM data at the given offset.
 * Throws an exception if the offset is out of range.
 */
BYTE *sms_rom::pointerAt(const uint addr)
{
    if(addr >= size)
    {
        throw std::out_of_range("Address out of range for ROM file");
    }
    else
    {
        return &data[addr];
    }
}

/*
 * Returns byte stored at address in ROM.
 * Throws an exception if the offset is out of range.
 */
BYTE sms_rom::read(const uint addr)
{
    if(addr >= size)
    {
        throw std::out_of_range("Address out of range for ROM file");
    }
    else
    {
        return data[addr];
    }
}

/*
 * Reads file from the given filename into memory.
 */
void sms_rom::read_file(std::string filename)
{
    uint filesize = 0;
    std::cout << "Reading ROM file" << filename << std::endl;
    std::ifstream file (filename, std::ios::in | std::ios::binary | std::ios::ate);
    if(file.is_open())
    {
        try
        {
            filesize = file.tellg();
            // Skip 512 byte header created by some ROM dumpers
            if(filesize % ROM_PAGE_SIZE == 512)
            {
                filesize -= 512;
                file.seekg(512, std::ios::beg);
            }
            else file.seekg(0, std::ios::beg);
            size = filesize;
            // Extra padding for Sega Card games
            // Avoids excess computation for mapping them to memory
            if(size < 0x20000) size = 0x20000;
            data = new BYTE[size];
            file.read((char *)data, filesize);
            file.close();
        }
        catch(std::ios::failure const& e)
        {
            file.close();
            std::cerr << "Read failed: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Read failed!" << std::endl;
        return;
    }
    if(size > 0x80000) onemeg = true;
    check_codemasters();
    std::cout << filesize << " bytes read successfully!" << std::endl;
    const char *header = "TMR SEGA";
    if(memcmp(&data[0x7ff0], header, 8) == 0)
    {
        std::cout << "Valid header found!\n" << std::endl;
    }
    else
    {
        std::cout << "ROM header invalid!  " << &data[0x7ff0] << " != " << header << "\nJapanese ROM or homebrew?" << std::endl;
    }

}

/*
 * Checks if the given ROM uses the CodeMasters memory mapper and sets the
 * codemasters variable if true.
 */
void sms_rom::check_codemasters()
{
    WORD checksum = ((data[0x7fe7] << 8) | data[0x7fe6]);
    if(checksum != 0)
    {
        WORD checksum_opposite = ((data[0x7fe9] << 8) | data[0x7fe8]);
        codemasters = ((0x10000 - checksum) == checksum_opposite);
    }
}
