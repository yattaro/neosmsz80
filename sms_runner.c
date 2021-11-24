#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mem.h"
#include "sms_runner.h"
#include "z80.h"

bool quitting = false;
struct mem_map *mem;
struct bin_object *bios;
struct bin_object *rom;

void sigint_handler()
{
    quitting = true;
    printf("\nInterrupted\n");
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

void system_init(const char *bios_file, const char *rom_file)
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
        fprintf(stderr, "No BIOS file specified, skipping...\n");
    }
    if(init_rom(rom_file) != 0) return;
    init_memory();
    printf("Memory map initialized:\n"
        "Slot 0: 0x%x-0x%x Slot 1: 0x%x-0x%x Slot 2: 0x%x-0x%x\n"
        "RAM: 0x%x-0x%x RAM Mirror: 0x%x-0x%x\n"
        "Total memory: %d bytes\n",
        ROM_S0_OFFSET, ROM_S1_OFFSET-1, ROM_S1_OFFSET,
        ROM_S2_OFFSET-1, ROM_S2_OFFSET, RAM_OFFSET-1,
        RAM_OFFSET, RAM_MIRROR_OFFSET-1,
        RAM_MIRROR_OFFSET, MEM_SIZE-1, MEM_SIZE);
    signal(SIGINT, sigint_handler);
    errno = run_loop();
}

int init_rom(const char *rom_file)
{
    if(rom_file != NULL)
        {
        rom = read_bin_file(rom_file);
        if(errno == 0 && rom_file != NULL)
        {
            printf("ROM file %s: %d bytes read successfully.\n",
                rom_file, rom->size);
            const char *header = "TMR SEGA";
            if(memcmp(&rom->data[0x7ff0], header, 8) == 0)
            {
                printf("Valid header found!\n");
            }
            else
            {
                printf("ROM header invalid!  %.*s != %s\nJapanese ROM?\n",
                8, &rom->data[0x7ff0], header);
            }
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