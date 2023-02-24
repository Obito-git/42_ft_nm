#ifndef NM_ERRORS_H
#define NM_ERRORS_H

#define ERR_OPEN_FILE(path) (path ": error: Can't read this file")
#define ERR_CLOSE_FILE(path) (path ": error: Can't close this file")
#define ERR_MMAP "mmap(2) failure. exiting..."
#define ERROR_MUNMAP "munmap(2) failure. exiting..."

#endif //NM_ERRORS_H
