#include "ft_nm.h"

char get_symbol_type(int symtype, int symbind, int symvis) {
    (void) symbind;
    (void) symtype;
    (void) symvis;
    /*
    if (symtype == STT_FUNC) {
        if (symbind == STB_GLOBAL) {
            if (symvis == STV_DEFAULT)
                return 't';
            if (symvis == STV_HIDDEN)
                return 'T';
            return 'V';
        }
        return 'u';
    }
    */
//    Elf64_Sym *sym = (Elf64_Sym *) ((char *) map_start + symtab_shdr->sh_offset + i * sizeof(Elf64_Sym));


    return '?';
}


void print_details(file_details_t *details) {
    symbol_t sym;
    for (size_t i = 0; i < details->table_det.symbols_number; i++) {
        setSymbolDetails(details, &sym);
        if (isLegalSymbolTableType(&details->table_det, &sym)) {


            /*
            Elf64_Shdr *shdr = (Elf64_Shdr *) ((char *) details->file_start +
                                               ((Elf64_Ehdr *) details->file_start)->e_shoff);
            Elf64_Shdr *section = &shdr[sym.st_shndx];
            Elf64_Shdr *strtab_shdr = &shdr[((Elf64_Ehdr *) details->file_start)->e_shstrndx];
            char *strtab = (char *) (details->file_start + strtab_shdr->sh_offset);
            char *section_name = strtab + section->sh_name;

            */


            printf("  %03zu:\t%016lx\t%c\t\t%s\t\t%s\n", i, sym.st_value,
                   get_symbol_type(ELF64_ST_TYPE(sym.st_info), ELF64_ST_BIND(sym.st_info),
                                   ELF64_ST_VISIBILITY(sym.st_info)),
                   details->table_det.strtab + sym.st_name,
                   "");
        }
    }
}