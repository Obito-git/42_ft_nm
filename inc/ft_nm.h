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

#define MIN_ELF_BYTES 6 //FIXME find correct value

enum Sort {SORT_YES, SORT_REVERSE, SORT_NO};
enum Display {DISPLAY_NORM, DISPLAY_ALL, DISPLAY_EXTERNAL, DISPLAY_UNDEFINED};

/* printed symbol data */
typedef struct elf_symbol {
    char        *name;      /*  symbol name */
    uint64_t    addr_val;   /*  symbol address */
    char        nm_type;    /*  nm symbol type */
} elf_symbol;

typedef struct symbol_table_info {
    bool        is64bit;               /* architecture */
    void        *symtab;               /* pointer to symbol table */
    void        *section_header_start; /* pointer to the first section */
    size_t      symbols_number;        /* number of symbols in symbol table */
    char        *strtab;               /* pointer to table of symbol names */
    size_t      added_symbol_count;    /* parsed symbols count in **symbols */
    elf_symbol  **symbols;             /* parsed symbols array */
} symbol_table_info;


/* handle64.c */
int handle64(Elf64_Ehdr *elf_header, symbol_table_info *symbol_table_info);

/* errors.c */
int handle_error(const char *msg, int error_code);
int handle_error_prefix(const char *msg, const char *prefixMsg, int error_code);

char symbol_nm_type(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx, uint32_t section_type,
                    uint32_t section_flags);
void symbolSort(elf_symbol **arr, int low, int high, enum Sort sort);
void print_symbols(const symbol_table_info *table_info, enum Display display);

#endif //FT_NM_H


