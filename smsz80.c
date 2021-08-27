#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include "sms_runner.h"

void print_usage(char *prog_name)
{
    printf(
        "Usage: %s --bios=<BIOS file> <ROM file>\n"
        "-h,  --help                            Print this help.\n"
        "-b <BIOS file>,  --bios=<BIOS file>    Specifies BIOS file to use.\n"
        "-r <ROM file>,   --rom=<ROM file>      Specifies ROM file to use.\n",
        prog_name
    );
}

int main(int argc, char *argv[])
{
    char *bios_file = NULL, *rom_file = NULL;
    bool help_flag = false;
    int c = 0, longind = 0;
    struct option longopts[] =
    {
        {"help", no_argument,       0, 'h'},
        {"bios", required_argument, 0, 'b'},
        {"rom",  required_argument, 0, 'r'},
        {0,      0,                 0,  0 }
    };
    char *shortopts = "-hb:r:";
    while(!help_flag &&
        (c = getopt_long(argc, argv, shortopts, longopts, &longind)) != -1
    )
    {
        switch(c)
        {
            case 'b':
                bios_file = optarg;
                break;
            default:
                /* Interpret non-option argument as ROM file */
                /* fall through */
            case 'r':
                if(rom_file == NULL)
                {
                    rom_file = optarg;
                    break;
                }
                printf("More than one ROM file specified!\n");
                /* fall through */
            case '?':
                errno = EINVAL;
                /* fall through */
            case 'h':
                help_flag = true;
                break;
        }
    }
    if(help_flag) print_usage(argv[0]);
    else system_init(argv[0], bios_file, rom_file);
    return errno;
}