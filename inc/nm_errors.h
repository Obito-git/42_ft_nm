#ifndef NM_ERRORS_H
#define NM_ERRORS_H

#include "libft.h"

#define ERRMSG_OPEN_FILE "Error. Can't open file\n"
#define ERRMSG_IS_DIR "Error. File is directory\n"
#define ERRMSG_FSTAT "Error. Can't execute fstat\n"
#define ERRMSG_MMAP "mmap(2) failure. exiting...\n"
#define ERRMSG_NOTELF "Error. You must provide an elf file\n"

#define ERRMSG_CLOSE_FILE(path) (path ": error: Can't close this file")

#define ERRORMSG_MUNMAP "munmap(2) failure. exiting..."


#define ERRMSG_ARGS_COUNT "You can't provide more than 1 argument"

#endif //NM_ERRORS_H
