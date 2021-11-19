#include <stdbool.h>
#ifndef SMSZ80_COMMON
#define SMSZ80_COMMON

typedef unsigned char BYTE;
typedef unsigned short int WORD;
typedef signed short int SIGNED_WORD;
typedef signed char signed_byte;

#ifndef SYS_CLKS
#define SYS_CLKS
// Values below are for NTSC Sega Master System
// Main system clock is approximately 10.738580MHz
#define MAIN_CLK 10738580
// Only providing dividers for Z80, VDP, and video clocks because preprocessor
// can't do floating point arithmetic
#define CORE_DIV 3
#define VDP_DIV 2
#define TICKS_PER_FRAME 179208
#endif

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
    bool onemeg;
    bool codemasters;
    BYTE *data;
};

extern struct mem_map *mem;
extern struct bin_object *bios;
extern struct bin_object *rom;

#endif