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
        bin = NULL;
        //set errno for empty files
        if(filesize == 0) errno = EBFONT;
    }
    else
    {
        bin->size = filesize;
        bin->data = malloc(bin->size);
        size_t read_bytes = fread(bin->data, 1, bin->size, fp);
        if(errno)
        {
            fclose(fp);
            free(bin->data);
            free(bin);
            bin = NULL;
        }
        else if(read_bytes != filesize)
        {
            fclose(fp);
            free(bin->data);
            free(bin);
            bin = NULL;
            errno = EIO;
        }
    }

    return bin;
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