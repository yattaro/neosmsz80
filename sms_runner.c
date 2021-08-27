#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mem.h"
#include "sms_runner.h"

struct mem_map *mem;
struct bin_object *bios;
struct bin_object *rom;
bool codemasters = false;

void update()
{
    //signal other threads
}

void run_loop()
{
    while(1)
    {
        
    }
}

void system_init(char *prog_name, char *bios_file, char *rom_file)
{
    if(bios_file != NULL)
    {
        bios = read_bin_file(bios_file);
        if(errno == 0 && bios != NULL)
        {
            printf("BIOS file %s: %d bytes read successfully.\n",
                bios_file, bios->size);
        }
        else
        {
            perror("Failed to read BIOS file.");
            return;
        }
    }
    else
    {
        fprintf(stderr, "No BIOS file specified!\nUse %s --help for help.\n",
            prog_name
        );
        errno = EINVAL;
        return;
    }
    if(init_rom(rom_file) != 0) return;
    mem = init_memory();
    printf("Memory map initialized:\n"
        "Slot 0: 0x%x-0x%x Slot 1: 0x%x-0x%x Slot 2: 0x%x-0x%x\n"
        "RAM: 0x%x-0x%x RAM Mirror: 0x%x-0x%x\n"
        "Total memory: %d bytes\n",
        mem->rom_s0_offset, (mem->rom_s1_offset)-1, mem->rom_s1_offset,
        (mem->rom_s2_offset)-1, mem->rom_s2_offset, (mem->ram_offset)-1,
        mem->ram_offset, (mem->ram_mirror_offset)-1,
        mem->ram_mirror_offset, (mem->size)-1, mem->size);
}

int init_rom(char *rom_file)
{
    if(rom_file != NULL)
        {
        rom = read_bin_file(rom_file);
        if(errno == 0 && rom_file != NULL)
        {
            printf("ROM file %s: %d bytes read successfully.\n",
                rom_file, rom->size);
        }
        else
        {
            perror("Failed to read ROM file.");
            return 1;
        }
    }
    else printf("No ROM file specified!  Booting standalone BIOS!\n");
    return 0;
}