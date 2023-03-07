#include "ft_nm.h"

void symbolSort(symbol_t arr[], int low, int high) {
    int pivot, i, j;
    symbol_t temp;
    if (low < high) {
        pivot = low;
        i = low;
        j = high;
        while (i < j) {
            while (ft_strcmp(arr[i].section_name, arr[pivot].section_name) <= 0  && i <= high) {
                i++;
            }
            while (ft_strcmp(arr[j].section_name, arr[pivot].section_name) > 0  && j >= low) {
                j--;
            }
            if (i < j) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        temp = arr[j];
        arr[j] = arr[pivot];
        arr[pivot] = temp;
        symbolSort(arr, low, j - 1);
        symbolSort(arr, j + 1, high);
    }
}

void setSymbolDetails(file_details_t *details, symbol_t *sym_det) {

        if (details->table_det.is64bit) {
            Elf64_Sym *sym = details->table_det.symtab;
            sym_det->st_name = sym->st_name;
            sym_det->st_info = sym->st_info;
            sym_det->st_value = sym->st_value;

            if (isLegalSymbolTableType(&details->table_det, sym_det)) {
                sym_det->section = &details->shdr64[sym->st_shndx];
                Elf64_Shdr *strtab_shdr = &details->shdr64[((Elf64_Ehdr *) details->file_start)->e_shstrndx];
                char *strtab = (char *) (details->file_start + strtab_shdr->sh_offset);
                ft_strcpy(sym_det->section_name, strtab + ((Elf64_Shdr *) sym_det->section)->sh_name);
            }



            details->table_det.symtab += sizeof(Elf64_Sym);
            return;
        }
        /*
        Elf32_Sym *sym = table_det->symtab;
        sym_det->st_name = sym->st_name;
        sym_det->st_info = sym->st_info;
        sym_det->st_value = sym->st_value;
        sym_det->st_shndx = sym->st_shndx;
        table_det->symtab += sizeof(Elf32_Sym);
         */
}


bool isLegalSymbolTableType(table_details_t *table_det, symbol_t *sym_det) {
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