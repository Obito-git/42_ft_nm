#ifndef NM_ERRORS_H
#define NM_ERRORS_H

#include "libft.h"

#define NAME "ft_nm: "

#define ERRMSG_OPEN_FILE "Error. Can't open file\n"
#define ERRMSG_IS_DIR "Error. File is directory\n"
#define ERRMSG_FSTAT "Error. Can't execute fstat\n"
#define ERRMSG_MMAP "mmap(2) failure. exiting...\n"
#define ERRMSG_NOTELF "Error. You must provide an elf file\n"
#define ERRMSG_FILEFORMAT "file format not recognized"
#define ERRMSG_MALLOC "malloc error. Can't allocate memory"
#define ERRMSG_NOSYMTAB "symtab is not found in elf"



#define ERRMSG_CLOSE_FILE(path) (path ": error: Can't close this file")

#define ERRORMSG_MUNMAP "munmap(2) failure. exiting..."




#define FT_NM_USAGE "Usage: nm [option(s)] [file(s)]\n\
 List symbols in [file(s)] (a.out by default).\n\
 The options are:\n\
  -a                     Display debugger-only symbols\n\
  -g                     Display only external symbols\n\
  -p                     Do not sort the symbols\n\
  -r                     Reverse the sense of the sort\n\
  -u                     Display only undefined symbols\n\
nm: supported targets: elf64-x86-64 elf32-i386 elf32-iamcu \
elf32-x86-64 pei-i386 pe-x86-64 pei-x86-64 elf64-l1om elf64-k1om \
elf64-little elf64-big elf32-little elf32-big pe-bigobj-x86-64 pe-i386 \
srec symbolsrec verilog tekhex binary ihex plugin\n"

#define PRINT_UNKNOWN_ARG_ERROR(arg) ft_fprintf(STDERR_FILENO, "%sinvalid option -- \'%c\'\n%s", NAME, (arg), FT_NM_USAGE)

#endif //NM_ERRORS_H
