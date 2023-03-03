#include "ft_nm.h"

char get_symbol_type(int symtype, int symbind, int symvis) {
    if (symtype == STT_NOTYPE) {
        if (symbind == STB_GLOBAL)
            return 'c';
        if (symbind == STB_LOCAL)
            return 'l';
        return 'u';
    }
    if (symtype == STT_OBJECT) {
        if (symbind == STB_GLOBAL) {
            if (symvis == STV_DEFAULT)
                return 'd';
            if (symvis == STV_HIDDEN)
                return 'D';
            return 'V';
        }
        if (symbind == STB_LOCAL) {
            if (symvis == STV_DEFAULT)
                return 'd';
            return 't';
        }
        return 'u';
    }
    if (symtype == STT_FUNC) {
        if (symbind == STB_GLOBAL) {
            if (symvis == STV_DEFAULT)
                return 't';
            if (symvis == STV_HIDDEN)
                return 'T';
            return 'V';
        }
        if (symbind == STB_LOCAL) {
            if (symvis == STV_DEFAULT)
                return 't';
            return 'i';
        }
        return 'u';
    }
    if (symtype == STT_TLS) {
        if (symbind == STB_GLOBAL)
            return 'd';
        if (symbind == STB_LOCAL)
            return 't';
        return 'u';
    }
    return '?';
}


void print_details(file_details_t *details) {
    symbol_details_t sym;
    size_t size = details->table_det.is64bit ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym);
    for (size_t i = 0; i < details->table_det.symsize / size; i++) {
        setSymbolDetails(&details->table_det, &sym);
        if (isLegalSymbolTableType(&details->table_det, &sym)) {
            printf("  %03zu:\t%016lx\t%c\t\t%s\n", i, sym.st_value,
                   get_symbol_type(ELF64_ST_TYPE(sym.st_info), ELF64_ST_BIND(sym.st_info),
                                   ELF64_ST_VISIBILITY(sym.st_info)),
                   details->table_det.strtab + sym.st_name);
        }
    }
}