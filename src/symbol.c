#include "ft_nm.h"

void setSymbolDetails(table_details_t *table_det, symbol_details_t *sym_det) {
    if (table_det->is64bit) {
        Elf64_Sym *sym = table_det->symtab;
        sym_det->st_name = sym->st_name;
        sym_det->st_info = sym->st_info;
        sym_det->st_value = sym->st_value;
        table_det->symtab += sizeof(Elf64_Sym);
        return;
    }
    Elf32_Sym *sym = table_det->symtab;
    sym_det->st_name = sym->st_name;
    sym_det->st_info = sym->st_info;
    sym_det->st_value = sym->st_value;
    table_det->symtab += sizeof(Elf32_Sym);
}


bool isLegalSymbolTableType(table_details_t *table_det, symbol_details_t *sym_det) {
    if (table_det->is64bit)
        return (ELF64_ST_TYPE(sym_det->st_info) == STT_FUNC ||
                ELF64_ST_TYPE(sym_det->st_info) == STT_OBJECT ||
                ELF64_ST_TYPE(sym_det->st_info) == STT_NOTYPE) &&
               ft_strlen(sym_det->st_name + table_det->strtab);
    return (ELF32_ST_TYPE(sym_det->st_info) == STT_FUNC ||
            ELF32_ST_TYPE(sym_det->st_info) == STT_OBJECT ||
            ELF32_ST_TYPE(sym_det->st_info) == STT_NOTYPE) &&
           ft_strlen(sym_det->st_name + table_det->strtab);

}