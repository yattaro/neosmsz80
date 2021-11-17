#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "mem.h"
#include "sms_runner.h"
#include "z80.h"

bool quitting = false;
struct mem_map *mem;
struct bin_object *bios;
struct bin_object *rom;
bool codemasters = false;

void sigint_handler()
{
    quitting = true;
    printf("Interrupted\n");
}

double clock_gettime_msec()
{
    struct timespec current;
    if(clock_gettime(CLOCK_MONOTONIC, &current))
    {
        return -1;
    }
    return (current.tv_sec * 1000) + round(current.tv_nsec / 1000000);
}

void update()
{
    unsigned int ticks = 0;

    while(ticks < TICKS_PER_FRAME)
    {
        int core_cycles = next_instruction();
        int new_ticks = core_cycles * CORE_DIV;
        // TODO: add VDP and sound updates, not immediately necessary
        // for testing the event loop though
        ticks += new_ticks;
    }
}

int run_loop()
{
    int exit_status = 0;
    //target framerate in frames per millisecond
    const double frame_rate = 1000 / (MAIN_CLK/TICKS_PER_FRAME);
    double last_frame_time = 0;
    double current_time = 0;
    while(!quitting && exit_status == 0)
    {
        if((current_time = clock_gettime_msec()) == -1)
        {
            exit_status = errno;
            break;
        }
        if((last_frame_time + frame_rate) <= current_time)
        {
            last_frame_time = current_time;
            update();
        }
    }
    return exit_status;
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
    signal(SIGINT, sigint_handler);

    int exit_status = run_loop();
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