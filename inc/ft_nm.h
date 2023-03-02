#ifndef FT_NM_H
#define FT_NM_H

#include "nm_errors.h"
#include "libft.h"
#include <fcntl.h>
#include <elf.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h> //FIXME DELETE

#define MIN_ELF_BYTES 6

typedef struct table_details {
    bool is64bit;
    Elf32_Shdr *symbHdr32;
    Elf64_Shdr *symbHdr64;
    void *symtab;
    size_t symsize;
    char *strtab;
    size_t strsize;
} table_details_t;

typedef struct file_details {
    int fd;
    void *file_start;
    struct stat buf;
    Elf32_Ehdr *ehdr32;
    Elf64_Ehdr *ehdr64;
    table_details_t table_det;
} file_details_t;



/* file_manip.c */
int parseFileToDetails(char *filename, file_details_t *details);
void fillHeadersAndSymbolTable(file_details_t *details);

/* errors.c */
int handle_error(char *msg, int error_code);
int handle_error_prefix(char *msg, char *prefixMsg, int error_code);

#endif //FT_NM_H
