#include "ft_nm.h"

/* 32bit
 * Parsing symbol table details and filling table_details_t
 *
 * @argument details is struct defined in ft_nm.h
 * @argument secNumb elf header section number
 * @argument sectionHdr32 is pointer on 32bit section header
 * @return //FIXME add return (error case)
 */
void fillSymbolTable32(file_details_t *details) {
    table_details_t *table_det = &details->table_det;

    for (int i = 0; i < details->ehdr32->e_shnum; i++) {
        if (details->shdr32[i].sh_type == SHT_SYMTAB) {
            table_det->symtab = details->file_start + details->shdr32[i].sh_offset;
            table_det->symsize = details->shdr32[i].sh_size;
            table_det->strtab = (char *) (details->file_start + details->shdr32[details->shdr32[i].sh_link].sh_offset);
            table_det->symbols_number = table_det->symsize / sizeof(Elf32_Sym);
            //table_det->strsize = details->shdr32[details->shdr32[i].sh_link].sh_size;
            break;
        }
        //FIXME error_handling
    }
}


void setSymbolDetails32(file_details_t *details, symbol_t *sym_det) {
    if (!details->table_det.is64bit) {
        Elf32_Sym *sym = details->table_det.symtab;
        sym_det->st_name = sym->st_name;
        sym_det->st_info = sym->st_info;
        sym_det->st_value = sym->st_value;

        if (isLegalSymbolTableType(&details->table_det, sym_det)) {
            sym_det->symbol_name = ft_strdup(details->table_det.strtab + sym_det->st_name);
            sym_det->section = &details->shdr32[sym->st_shndx];
            //Elf32_Shdr *strtab_shdr = &details->shdr32[((Elf32_Ehdr *) details->file_start)->e_shstrndx];
            //char *strtab = (char *) (details->file_start + strtab_shdr->sh_offset);
            //ft_strcpy(sym_det->section_name, strtab + ((Elf32_Shdr *) sym_det->section)->sh_name);
        }
        details->table_det.symtab += sizeof(Elf32_Sym);
        return;
    }
}