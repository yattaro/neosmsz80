#include <stdio.h>
#include "smsz80_common.h"

#ifndef MEM
#define MEM

#define ROM_S0_SIZE 0x4000
#define ROM_S1_SIZE 0x4000
#define ROM_S2_SIZE 0x4000
#define RAM_SIZE 0x2000
#define RAM_MIRROR_SIZE 0x2000

struct mem_map
{
    int rom_s0_offset;
    int rom_s1_offset;
    int rom_s2_offset;
    int ram_offset;
    int ram_mirror_offset;
    int size;
    BYTE *data;
};

// struct used to store binary data from files
struct bin_object{
    int size;
    BYTE *data;
};

struct mem_map *init_memory();
struct bin_object *read_bin_file(char *filename);
size_t get_file_size(FILE *fp);
#endif