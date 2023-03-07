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
    size_t symbols_number;
    char *strtab;
    int64_t st_name;
    //size_t strsize;
} table_details_t;

typedef struct symbol {
    int64_t st_name;
    unsigned char	st_info;		/* Symbol type and binding */
    uint64_t st_value;
    void *section;
    char section_name[100];
} symbol_t;

typedef struct file_details {
    int fd;                     /* File descriptor */
    void *file_start;           /* Pointer to file begin */
    struct stat file_stat;      /* File stat */
    Elf32_Ehdr *ehdr32;         /* Elf header 32bit */
    Elf64_Ehdr *ehdr64;         /* Elf header 64bit */
    Elf32_Shdr *shdr32;         /* First section 32bit header pointer */
    Elf64_Shdr *shdr64;         /* First section 64bit header pointer */
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
void setSymbolDetails(file_details_t *details, symbol_t *sym_det);
bool isLegalSymbolTableType(table_details_t *table_det, symbol_t *sym_det);
void symbolSort(symbol_t arr[], int low, int high);

#endif //FT_NM_H
