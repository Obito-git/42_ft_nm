#include "ft_nm.h"

static int read_symbol_table32(symbol_table_info *table_info) {
    for (size_t i = 0; i < table_info->symbols_number; i++) {
        Elf32_Sym *symbol_header = table_info->symtab;
        Elf32_Shdr *section_header = &(((Elf32_Shdr *) table_info->section_header_start)[symbol_header->st_shndx]);
        if (ELF32_ST_TYPE(symbol_header->st_info) != STT_FILE && ELF32_ST_TYPE(symbol_header->st_info) != STT_SECTION) {
            char type = get_nm_symbol_type(ELF32_ST_BIND(symbol_header->st_info),
                                           ELF32_ST_TYPE(symbol_header->st_info),
                                           symbol_header->st_shndx, section_header->sh_type,
                                           section_header->sh_flags);
            elf_symbol *symbol = elf_constructor(symbol_header->st_name + table_info->strtab,
                                                 symbol_header->st_value,
                                                 type);
            if (!symbol)
                return handle_error_prefix(ERRMSG_MALLOC, "elf_symbol: ", EXIT_FAILURE);
            if (!ft_strlen(symbol->name) && table_info->sort != SORT_NO)
                table_info->noname_symb[table_info->added_noname_count++] = symbol;
            else
                table_info->symbols[table_info->added_symbol_count++] = symbol;
        }
        table_info->symtab += sizeof(Elf32_Sym);
    }
    return EXIT_SUCCESS;
}

int handle32(Elf32_Ehdr *elf_header, symbol_table_info *symbol_table_info, const char *filename) {
    void *file_start = ((void *) elf_header);
    Elf32_Shdr *section_header = (Elf32_Shdr *) (file_start + elf_header->e_shoff);
    symbol_table_info->section_header_start = section_header;
    for (int i = 0; i < elf_header->e_shnum; i++) {
        if (section_header[i].sh_type == SHT_SYMTAB) {
            symbol_table_info->symtab = file_start + section_header[i].sh_offset;
            symbol_table_info->strtab = (char *) (file_start + section_header[section_header[i].sh_link].sh_offset);
            symbol_table_info->symbols_number = section_header[i].sh_size / sizeof(Elf32_Sym);
            if (!(symbol_table_info->symbols = malloc(sizeof(elf_symbol *) * symbol_table_info->symbols_number)))
                return handle_error_prefix(ERRMSG_MALLOC, "elf_symbol*: ", EXIT_FAILURE);
            if (!(symbol_table_info->noname_symb = malloc(sizeof(elf_symbol *) * symbol_table_info->symbols_number))) {
                free(symbol_table_info->symbols);
                return handle_error_prefix(ERRMSG_MALLOC, "elf_symbol*: ", EXIT_FAILURE);
            }
            return read_symbol_table32(symbol_table_info);
        }
    }
    return handle_error_prefix(ERRMSG_NOSYMTAB, filename, EXIT_FAILURE);
}