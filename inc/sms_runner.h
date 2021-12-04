#ifndef SMS_RUNNER
#define SMS_RUNNER

void sigint_handler();
double clock_gettime_msec();
void update();
int run_loop();
void system_init(const char *bios_file, const char *rom_file);
int init_rom(const char *rom_file);


#endif