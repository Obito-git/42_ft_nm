#ifndef NM_ERRORS_H
#define NM_ERRORS_H

#include "libft.h"

#define ERRMSG_OPEN_FILE(path) ft_fprintf(STDERR_FILENO, "%s: error can't open file\n", path)
#define ERRMSG_FSTAT ft_fprintf(STDERR_FILENO, "Error can't execute fstat\n")
#define ERRMSG_CLOSE_FILE(path) (path ": error: Can't close this file")
#define ERRMSG_MMAP ft_fprintf(STDERR_FILENO, "mmap(2) failure. exiting...")
#define ERRORMSG_MUNMAP "munmap(2) failure. exiting..."

#define ERRMSG_ARGS_COUNT "You can't provide more than 0 argument"

#endif //NM_ERRORS_H
