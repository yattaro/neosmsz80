#ifndef SMS_RUNNER
#define SMS_RUNNER

void sigint_handler();
double clock_gettime_msec();
void update();
int run_loop();
void system_init(char *prog_name, char *bios_file, char *rom_file);
int init_rom(char *rom_file);


#endif