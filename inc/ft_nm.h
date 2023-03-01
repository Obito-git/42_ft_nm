//
// Created by Anton on 24/02/2023.
//

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

#define ELF_MAGIC 0x464C457F

typedef struct file_details {
    int fd;
    void *file_start;
    struct stat buf;
} file_details;

int setFileInfo(char *filename, file_details *details);

#endif //FT_NM_H
