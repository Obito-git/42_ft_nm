#ifndef NM_ERRORS_H
#define NM_ERRORS_H

#include "libft.h"

#define NAME "ft_nm: "

#define ERRMSG_OPEN_FILE "Error. Can't open file\n"
#define ERRMSG_IS_DIR "Error. File is directory\n"
#define ERRMSG_FSTAT "Error. Can't execute fstat\n"
#define ERRMSG_MMAP "mmap(2) failure. exiting...\n"
#define ERRMSG_NOTELF "Error. You must provide an elf file\n"
#define ERRMSG_FILEFORMAT "file format not recognized\n"
#define ERRMSG_MALLOC "malloc error. Can't allocate memory\n"
#define ERRMSG_NOSYMTAB "no symbols or file format not recognized\n"


#define FT_NM_USAGE "Usage: nm [option(s)] [file(s)]\n\
 List symbols in [file(s)] (a.out by default).\n\
 The options are:\n\
  -a                     Display debugger-only symbols\n\
  -g                     Display only external symbols\n\
  -p                     Do not sort the symbols\n\
  -r                     Reverse the sense of the sort\n\
  -u                     Display only undefined symbols\n\
nm: supported targets: elf-x86_32, elf-x64, object files, .so\n"

#define PRINT_UNKNOWN_ARG_ERROR(arg) ft_fprintf(STDERR_FILENO, "%sinvalid option -- \'%c\'\n%s", NAME, (arg), FT_NM_USAGE)

#endif //NM_ERRORS_H
