#include <stdio.h>
#include "smsz80_common.h"

#ifndef MEM
#define MEM

struct mem_map *init_memory();
struct bin_object *read_bin_file(char *filename);
void check_codemasters(struct bin_object *rom);
size_t get_file_size(FILE *fp);
#endif