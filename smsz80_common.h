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

#endif