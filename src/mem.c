#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "mem.h"
#include "smsz80_common.h"


/*
 * Initializes system memory and returns the memory map structure.
 * Returns a null pointer if this fails.
 */
void init_memory()
{
    // TODO: figure out how to make this more memory efficient; main_mem
    // includes the lower address space that is always mapped to cartridge ROM
    // or RAM, so this space is effectively unused other than for alignment
    mem = malloc(sizeof(struct mem_map));
    mem->main_mem = malloc(MEM_SIZE);
    mem->bank_0 = malloc(ROM_PAGE_SIZE);
    mem->bank_1 = malloc(ROM_PAGE_SIZE);
    mem->slot_0 = &rom->data[0];
    mem->slot_1 = &rom->data[ROM_PAGE_SIZE];
    mem->slot_2 = &rom->data[2*ROM_PAGE_SIZE];
}

/*
 * Checks if the given ROM uses the CodeMasters memory mapper and sets the
 * codemasters variable if true.
 */
void check_codemasters(struct bin_object *rom)
{
    WORD checksum = ((rom->data[0x7fe7] << 8) | rom->data[0x7fe6]);

    if(checksum == 0)
    {
        rom->codemasters = false;
    }
    else
    {
        WORD checksum_opposite = ((rom->data[0x7fe9] << 8) | rom->data[0x7fe8]);
        rom->codemasters = ((0x10000 - checksum) == checksum_opposite);
    }
}

/*
 * Determines if the memory segment indicated by addr is writable and stores the
 * value of data if it is, and simply returns if not (given this should be
 * undefined behavior on the actual hardware)
 */
void write_mem(const WORD addr, const BYTE data)
{
    if(rom->codemasters && (addr == 0x0 || addr == 0x4000 || addr == 0x8000))
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
        if(mem->main_mem[0xFFFC] & 0x8)
        {
            mem->slot_2[addr-ROM_S2_OFFSET] = data;
        }
        return;
    }

    // writing to RAM, should be fine
    mem->main_mem[addr] = data;
    
    // Writing to control registers, do standard paging if non-codemasters ROM
    if(addr >= 0xFFFC && !rom->codemasters)
    {
        mem_page(addr, data);
    }

    // RAM Mirror
    if(addr < 0xDFFC) mem->main_mem[addr+RAM_SIZE] = data;
    if(addr >= RAM_MIRROR_OFFSET) mem->main_mem[addr-RAM_SIZE] = data;

}

/*
 * Handles standard memory paging
 */
void mem_page(const WORD addr, const BYTE data)
{
    // Mask to first six bits if cartridge has 1MB ROM, first five if not
    BYTE page = rom->onemeg ? data & 0x3F : data & 0x1F;

    switch (addr)
    {
        // Set mapping of RAM to slot 2
        case 0xFFFC:
            switch (data & 0xC)
            {
                case 0x8:
                    mem->slot_2 = mem->bank_0;
                    break;
                case 0xC:
                    mem->slot_2 = mem->bank_1;
                    break;
                default:
                break;
            }
            break;
        
        // Slot 0
        case 0xFFFD:
            mem->slot_0 = &rom->data[ROM_PAGE_SIZE * page];
            break;

        // Slot 1
        case 0xFFFE:
            mem->slot_1 = &rom->data[ROM_PAGE_SIZE * page];
            break;
        
        // Mapping ROM to slot 2
        case 0xFFFF:
            if(!(mem->main_mem[0xFFFC] & 0x8))
            {
                mem->slot_2 = &rom->data[ROM_PAGE_SIZE * page];
            }
            break;
        default:
            break;
    }
}

/*
 * Handles memory paging for CodeMasters cartridges
 */
void mem_page_codemasters(const WORD addr, const BYTE data)
{
    // Mask to first six bits if cartridge has 1MB ROM, first five if not
    BYTE page = rom->onemeg ? data & 0x3F : data & 0x1F;
    switch (addr)
    {
        case 0x0:
            mem->slot_0 = &rom->data[ROM_PAGE_SIZE * page];
            break;
        case 0x4000:
            mem->slot_1 = &rom->data[ROM_PAGE_SIZE * page];
            break;
        case 0x8000:
            mem->slot_2 = &rom->data[ROM_PAGE_SIZE * page];
            break;
        default:
            break;
    }
}

/*
 * Returns value of memory stored at given address
 */
BYTE read_mem(const WORD addr)
{
    // Avoid reading the control registers by using the non-mirrored addresses
    if(addr >= 0xFFFC) return mem->main_mem[addr-0x2000];
    // Reading from lower memory
    if(addr < RAM_OFFSET)
    {
        // Memory below 0x400 in non-codemasters cartridges is always
        // mapped to the first page of ROM
        if(!rom->codemasters && addr < 0x400) return rom->data[addr];
    
        // Slot 0
        if(addr < ROM_S1_OFFSET) return mem->slot_0[addr];
        // Slot 1
        if(addr < ROM_S1_OFFSET) return mem->slot_1[addr - ROM_PAGE_SIZE];
        // Slot 2
        return mem->slot_2[addr - (2*ROM_PAGE_SIZE)];
    }

    return mem->main_mem[addr];
}

/*
 * Reads binary from the given file into memory; returns data in a bin_object; 
 * If an error occurred while reading the file, errno is set and a null pointer
 * is returned.
 */
struct bin_object *read_bin_file(const char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL) return NULL;

    struct bin_object *bin = malloc(sizeof(struct bin_object));

    size_t filesize = get_file_size(fp);
    if(filesize <= 0)
    {
        fclose(fp);
        free(bin);
        //set errno for empty files
        if(filesize == 0) errno = EBFONT;
        return NULL;
    }

    // Skip headers provided by ROM dumper programs
    if((filesize % ROM_PAGE_SIZE) == 512)
    {
        bin->size = filesize - 512;
        fseek(fp, 512, SEEK_SET);
    }
    else bin->size = filesize;

    bin->data = malloc(0x100000);
    size_t read_bytes = fread(bin->data, 1, bin->size, fp);
    if(errno)
    {
        fclose(fp);
        free(bin->data);
        free(bin);
        return NULL;
    }
    else if(read_bytes != filesize)
    {
        fclose(fp);
        free(bin->data);
        free(bin);
        errno = EIO;
        return NULL;
    }
    // Make appropriate checks for memory mapper
    if(bin->size > 0x80000) bin->onemeg = true;
    else bin->onemeg = false;
    check_codemasters(bin);

    return bin;
}

/*
 * Wrapper for fstat that returns the value of st_size for a file pointer or -1
 * if an error occurred while getting the size; the call to fstat sets errno.
 */
size_t get_file_size(FILE *fp)
{
    struct stat file_info;
    if(fstat(fp->_fileno, &file_info) == -1)
    {
        return -1;
    }
    return (size_t)file_info.st_size;
}