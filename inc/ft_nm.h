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

typedef struct file_details {
    int fd;
    void *file_start;
    struct stat buf;
} file_details;

/* file_manip.c */
int fillDetails(char *filename, file_details *details);
void parseFile(file_details *details);

/* errors.c */
int handle_error(char *msg, int error_code);
int handle_error_prefix(char *msg, char *prefixMsg, int error_code);

#endif //FT_NM_H
