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

/*
 * Contains details about symbol table
 * Elh32/64_Shdr - Symbol table header
 *
 * @var is64bit is architecture type
 * @var symtab is symbol table entry
 * @var symsize is size of symbol table
 * @var strtar is char representation of symbol table memory
 */
typedef struct table_details {
    bool is64bit;
    void *symtab;
    size_t symsize;
    char *strtab;
    int64_t st_name;
    //size_t strsize;
} table_details_t;

typedef struct symbol_details {
    int64_t st_name;
    unsigned char	st_info;		/* Symbol type and binding */
    uint64_t st_value;
} symbol_details_t;

typedef struct file_details {
    int fd;
    void *file_start;
    struct stat file_stat;
    table_details_t table_det;
} file_details_t;

/* file_manip.c */
int parseFileToDetails(char *filename, file_details_t *details);
void fillHeadersAndSymbolTable(file_details_t *details);

/* errors.c */
int handle_error(char *msg, int error_code);
int handle_error_prefix(char *msg, char *prefixMsg, int error_code);

/* print_result.c */
char get_symbol_type(int symtype, int symbind, int symvis);
void print_details(file_details_t *details);

/* symbol.c */
void setSymbolDetails(table_details_t *table_det, symbol_details_t *sym_det);
bool isLegalSymbolTableType(table_details_t *table_det, symbol_details_t *sym_det);

#endif //FT_NM_H
