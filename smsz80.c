#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "smsz80_common.h"

struct mem_map *mem;
struct bin_object *bios;
struct bin_object *rom;

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
            return;
        }
    }
    else printf("No ROM file specified!  Booting standalone BIOS!");
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

void run_loop()
{

}

void print_usage(char *prog_name)
{
    printf(
        "Usage: %s --bios=<BIOS file> --rom=<ROM file>\n"
        "-h,  --help                            Print this help.\n"
        "-b <BIOS file>,  --bios=<BIOS file>    Specifies BIOS file to use.\n"
        "-r <ROM file>,   --rom=<ROM file>      Specifies ROM file to use.\n",
        prog_name
    );
}

int main(int argc, char *argv[])
{
    char *bios_file = NULL;
    char *rom_file = NULL;
    int help_flag = 0, c = 0, option_index = 0;
    struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"bios", required_argument, 0, 'b'},
        {"rom", required_argument, 0, 'r'},
        {0,0,0,0}
    };
    while(!help_flag &&
        (c = getopt_long(argc, argv, "-hrb:", long_options, &option_index)) != -1
    )
    {
        switch(c)
        {
            case 'b':
                bios_file = optarg;
                break;
            case 'r':
                rom_file = optarg;
                break;
            default:
                errno = EINVAL;
                /* fall through */
            case 'h':
                help_flag = 1;
        }
    }
    if(help_flag) print_usage(argv[0]);
    else system_init(argv[0], bios_file, rom_file);
    return errno;
}