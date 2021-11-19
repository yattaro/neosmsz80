#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mem.h"
#include "smsz80_common.h"

/*
 * Initializes system memory and returns the memory map structure.
 * Returns a null pointer if this fails.
 */
struct mem_map *init_memory()
{
    struct mem_map *mem = malloc(sizeof(struct mem_map));
    mem->rom_s0_offset = 0;
    mem->rom_s1_offset = ROM_S0_SIZE;
    mem->rom_s2_offset = mem->rom_s1_offset + ROM_S1_SIZE;
    mem->ram_offset = mem->rom_s2_offset + ROM_S2_SIZE;
    mem->ram_mirror_offset = mem->ram_offset + RAM_SIZE;
    mem->size = mem->ram_mirror_offset + RAM_MIRROR_SIZE;
    mem->data = calloc(mem->size, sizeof(BYTE));
    return mem;
}

/*
 * Reads binary from the given file into memory; returns data in a bin_object; 
 * If an error occurred while reading the file, errno is set and a null pointer
 * is returned.
 */
struct bin_object *read_bin_file(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL) return NULL;

    struct bin_object *bin = malloc(sizeof(struct bin_object));

    size_t filesize = get_file_size(fp);
    if(filesize <= 0)
    {
        fclose(fp);
        free(bin);
        //set errno for empty files
        if(filesize == 0) errno = EBFONT;
        return NULL;
    }

    // Skip headers provided by ROM dumper programs
    if((filesize % 0x4000) == 512)
    {
        bin->size = filesize - 512;
        fseek(fp, 512, SEEK_SET);
    }
    else bin->size = filesize;

    bin->data = malloc(bin->size);
    size_t read_bytes = fread(bin->data, 1, bin->size, fp);
    if(errno)
    {
        fclose(fp);
        free(bin->data);
        free(bin);
        return NULL;
    }
    else if(read_bytes != filesize)
    {
        fclose(fp);
        free(bin->data);
        free(bin);
        errno = EIO;
        return NULL;
    }

    // Make appropriate checks for memory mapper
    if(bin->size > 0x80000) bin->onemeg = true;
    else bin->onemeg = false;
    check_codemasters(bin);

    return bin;
}


/*
 * Checks if the given ROM uses the CodeMasters memory mapper and sets the
 * codemasters variable if true.
 */
void check_codemasters(struct bin_object *rom)
{
    WORD checksum = ((rom->data[0x7fe7] << 8) | rom->data[0x7fe6]);

    if(checksum == 0)
    {
        rom->codemasters = false;
    }
    else
    {
        WORD checksum_opposite = ((rom->data[0x7fe9] << 8) | rom->data[0x7fe8]);
        rom->codemasters = ((0x10000 - checksum) == checksum_opposite);
    }
}

/*
 * Wrapper for fstat that returns the value of st_size for a file pointer or -1
 * if an error occurred while getting the size; the call to fstat sets errno.
 */
size_t get_file_size(FILE *fp)
{
    struct stat file_info;
    if(fstat(fp->_fileno, &file_info) == -1)
    {
        return -1;
    }
    return (size_t)file_info.st_size;
}