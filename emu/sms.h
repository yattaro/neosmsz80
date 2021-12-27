#ifndef SMS_H
#define SMS_H
#include <string>
#include <thread>
#include "z80mem.h"
#include "z80proc.h"

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


class sms
{
public:
    sms(z80mem *mem, z80proc *proc);
    sms(std::string rom_file);
    ~sms();
    z80mem *get_mem();
    z80proc *get_proc();
    bool isPaused();
    bool isStopped();
    void run();
    void pause();
    void stop();
    static void emulation_loop(sms *instance);
    void update();
private:
    z80mem *current_mem;
    z80proc *current_proc;
    bool external_mem = false;
    bool external_proc = false;
    bool paused = true;
    bool stopped = true;
    std::thread emulation_thread;
};

#endif // SMS_H
