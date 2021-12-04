#include <stdio.h>
#include "smsz80_common.h"

#ifndef MEM
#define MEM

void init_memory();
void check_codemasters(struct bin_object *rom);
void write_mem(const WORD addr, const BYTE data);
void mem_page(const WORD addr, const BYTE data);
void mem_page_codemasters(const WORD addr, const BYTE data);
BYTE read_mem(const WORD addr);
struct bin_object *read_bin_file(const char *filename);
size_t get_file_size(FILE *fp);
#endif