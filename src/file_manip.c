#include "ft_nm.h"

/*
 * Function is filling file_details structure
 * @argument filename is name of file
 * @argument details is struct defined in ft_nm.h
 * is where we will save file fd, file stat and pointer in file start
 * @return errno if an error occurred otherwise returns 0
 */
int fillDetails(char *filename, file_details *details) {
    if (details == NULL)
        return EINVAL;
    if ((details->fd = open(filename, O_RDONLY)) < 0)
        return handle_error_prefix(ERRMSG_OPEN_FILE, filename, errno);

    if ((fstat(details->fd, &details->buf)) < 0)
        return handle_error(ERRMSG_FSTAT, errno);

    if (S_ISDIR(details->buf.st_mode))
        return handle_error_prefix(ERRMSG_IS_DIR, filename, EISDIR);

    if (details->buf.st_size < MIN_ELF_BYTES)
        return handle_error_prefix(ERRMSG_NOTELF, filename, EXIT_FAILURE);

    if ((details->file_start = mmap(0,
                                    details->buf.st_size,
                                    PROT_READ,
                                    MAP_PRIVATE,
                                    details->fd,
                                    0)) == MAP_FAILED) {
        return handle_error(ERRMSG_MMAP, errno);
    }

    return 0;
}

void parseFile(file_details *details) {
    char *ptr = (char *) details->file_start;
    if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
        && ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3) {
        if (ptr[4] == ELFCLASS32) {
            ft_printf("32 bit elf binary\n");
        } else if (ptr[4] == ELFCLASS64) {
            ft_printf("64 bit elf binary\n");
        } else if (ptr[4] == ELFCLASSNONE){
            ft_printf("Unsupported elf class\n");
        } else {
            ft_printf("Not elf file\n");
        }
    }
}