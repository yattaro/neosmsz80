#ifndef SMS_RUNNER
#define SMS_RUNNER

void update();
void run_loop();
void system_init(char *prog_name, char *bios_file, char *rom_file);
int init_rom(char *rom_file);


#endif