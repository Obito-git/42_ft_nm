#include "ft_nm.h"

static void table_info_init(symbol_table_info *table_info) {
    table_info->symbols = NULL;
    table_info->noname_symb = NULL;
    table_info->added_symbol_count = 0;
    table_info->added_noname_count = 0;
}

static void table_info_destruct(symbol_table_info *table_info) {
    for (size_t i = 0; table_info->symbols && i < table_info->added_symbol_count; i++)
        free(table_info->symbols[i]);
    for (size_t i = 0; table_info->noname_symb && i < table_info->added_noname_count; i++)
        free(table_info->noname_symb[i]);
    if (table_info->symbols)
        free(table_info->symbols);
    if (table_info->noname_symb)
        free(table_info->noname_symb);
}

int handleElf(void *file_start, const char *file_name, bool print_name) {
    char *ptr = (char *) file_start;
    int ret = EXIT_SUCCESS;
    if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
        && ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3) {

        Elf32_Ehdr *elf_header32 = (Elf32_Ehdr *) ptr;
        Elf64_Ehdr *elf_header64 = (Elf64_Ehdr *) ptr;
        symbol_table_info table_info;
        table_info_init(&table_info);

        if (elf_header32->e_ident[EI_CLASS] == ELFCLASS32) {
            table_info.is64bit = false;
            ret = handle32(elf_header32, &table_info, file_name);
        } else if (elf_header64->e_ident[EI_CLASS] == ELFCLASS64) {
            table_info.is64bit = true;
            ret = handle64(elf_header64, &table_info, file_name);
        } else {
            return handle_error_prefix(ERRMSG_FILEFORMAT, file_name, EXIT_FAILURE);
        }
        if (ret == EXIT_SUCCESS) {
            symbolSort(table_info.symbols, 0, (int) table_info.added_symbol_count - 1);
            ret = print_symbols(&table_info, file_name, print_name);
        }
        table_info_destruct(&table_info);
    } else {
        return handle_error_prefix(ERRMSG_FILEFORMAT, file_name, EXIT_FAILURE);
    }
    return ret;
}