#ifndef FT_NM_H
#define FT_NM_H

#include "nm_errors.h"
#include "libft.h"
#include <fcntl.h>
#include <elf.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MIN_ELF_BYTES 6 //FIXME find correct value
#define HEX_64BIT_UINT_MAX_LEN 16
#define HEX_32BIT_UINT_MAX_LEN 8

enum Sort {SORT_YES, SORT_REVERSE, SORT_NO};
enum Display {DISPLAY_NORM, DISPLAY_ALL, DISPLAY_EXTERNAL, DISPLAY_UNDEFINED};

/* printed symbol data */
typedef struct elf_symbol {
    const char  *name;      /*  symbol name */
    uint64_t    addr_val;   /*  symbol address */
    char        nm_type;    /*  nm symbol type */
} elf_symbol;

typedef struct symbol_table_info {
    enum Sort     sort;                  /* sort type */
    enum Display  display;               /* display type */
    bool          is64bit;               /* architecture */
    void          *symtab;               /* pointer to symbol table */
    void          *section_header_start; /* pointer to the first section */
    size_t        symbols_number;        /* number of symbols in symbol table */
    char          *strtab;               /* pointer to table of symbol names */
    size_t        added_symbol_count;    /* parsed symbols count in **symbols */
    elf_symbol    **symbols;             /* parsed symbols array */
    size_t        added_noname_count;    /* parsed symbols count in **symbols but without name */
    elf_symbol    **noname_symb;         /* parsed symbols but without name */
} symbol_table_info;

/* handleAll.c */
int handleElf(void *file_start, const char *file_name, bool print_name, enum Sort sort, enum Display display);

/* handle64.c */
int handle64(Elf64_Ehdr *elf_header, symbol_table_info *symbol_table_info, const char *filename);

/* handle32.c */
int handle32(Elf32_Ehdr *elf_header, symbol_table_info *symbol_table_info, const char *filename);

/* errors.c */
int handle_error(const char *msg, int error_code);
int handle_error_prefix(const char *msg, const char *prefixMsg, int error_code);

/*  symbols.c*/
elf_symbol *elf_constructor(const char *name, uint64_t st_value, char type);
char get_nm_symbol_type(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx, uint32_t section_type,
                        uint32_t section_flags);

/* sort.c */
void symbolSort(elf_symbol **arr, int low, int high, enum Sort sort);

/* print.c */
int print_symbols(const symbol_table_info *table_info, const char *filename, bool print_name);

#endif //FT_NM_H


