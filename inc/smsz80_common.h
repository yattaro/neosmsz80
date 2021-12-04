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

// This looks ugly but it's a lot easier to me than memorizing the offsets
#define ROM_PAGE_SIZE 0x4000
#define ROM_S0_OFFSET 0x0
#define ROM_S1_OFFSET ROM_S0_OFFSET + ROM_PAGE_SIZE
#define ROM_S2_OFFSET ROM_S1_OFFSET + ROM_PAGE_SIZE
#define RAM_SIZE 0x2000
#define RAM_OFFSET ROM_S2_OFFSET + ROM_PAGE_SIZE
#define RAM_MIRROR_OFFSET RAM_OFFSET + RAM_SIZE
#define MEM_SIZE RAM_MIRROR_OFFSET + RAM_SIZE

// struct used to store binary data from files
struct bin_object{
    int size;
    bool onemeg;
    bool codemasters;
    BYTE *data;
};

// struct used to store main memory map and contents of catridge RAM banks
struct mem_map{
    BYTE *slot_0;
    BYTE *slot_1;
    BYTE *slot_2;
    BYTE *main_mem;
    BYTE *bank_0;
    BYTE *bank_1;
};

extern struct mem_map *mem;
extern struct bin_object *bios;
extern struct bin_object *rom;

#endif