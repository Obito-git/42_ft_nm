#include "ft_nm.h"

/*
 * Function is filling file_details structure
 * @argument filename is name of file
 * @argument details is struct defined in ft_nm.h
 * is where we will save file fd, file stat and pointer in file start
 * @return errno if an error occurred otherwise returns 0
 */
int setFileInfo(char *filename, file_details *details) {
    if (details == NULL)
        return EINVAL;
    if ((details->fd = open(filename, O_RDONLY)) < 0) {
        ERRMSG_OPEN_FILE(filename);
        return errno;
    }
    if ((fstat(details->fd, &details->buf)) < 0) {
        ERRMSG_FSTAT;
        return errno;
    }
    if ((details->file_start = mmap(0,
                                    details->buf.st_size,
                                    PROT_READ,
                                    MAP_PRIVATE,
                                    details->fd,
                                    0)) == MAP_FAILED) {
        ERRMSG_MMAP;
        return errno;
    }
    return 0;
}