#include "ft_nm.h"




void print_details(file_details_t *details) {
    symbol_t sym;
    for (size_t i = 0; i < details->table_det.symbols_number; i++) {
        setSymbolDetails64(details, &sym);
        if (isLegalSymbolTableType(&details->table_det, &sym)) {


            /*
            Elf64_Shdr *shdr = (Elf64_Shdr *) ((char *) details->file_start +
                                               ((Elf64_Ehdr *) details->file_start)->e_shoff);
            Elf64_Shdr *section = &shdr[sym.st_shndx];
            Elf64_Shdr *strtab_shdr = &shdr[((Elf64_Ehdr *) details->file_start)->e_shstrndx];
            char *strtab = (char *) (details->file_start + strtab_shdr->sh_offset);
            char *section_name = strtab + section->sh_name;

            */


        }
    }
}