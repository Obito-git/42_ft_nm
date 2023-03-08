#include "ft_nm.h"

static int ft_strcmp_ignore_underscore(const char *s1, const char *s2) {
    const char *s1_mod = (ft_strlen(s1) && s1[0] == '_') ? ft_strtrim(s1, "_") : s1;
    const char *s2_mod = (ft_strlen(s1) && s2[0] == '_') ? ft_strtrim(s2, "_") : s2;
    int res = ft_strcmp_ignore_case(s1_mod, s2_mod);
    if (s1 != s1_mod)
        free((char *) s1_mod);
    if (s2 != s2_mod)
        free((char *) s2_mod);
    return res;
}

static void symbolSort(symbol_t arr[], int low, int high) {
    int pivot, i, j;
    symbol_t temp;
    if (low < high) {
        pivot = low;
        i = low;
        j = high;
        while (i < j) {
            while (ft_strcmp_ignore_underscore(arr[i].symbol_name, arr[pivot].symbol_name) <= 0  && i <= high) {
                i++;
            }
            while (ft_strcmp_ignore_underscore(arr[j].symbol_name, arr[pivot].symbol_name) > 0  && j >= low) {
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

char get_symbol_type(const symbol_t *sym) {
    char res = '?';
    if (sym->st_shndx == SHN_ABS)
        res = 'A';
    else if (sym->sh_type & SHT_NOBITS && sym->sh_flags & SHF_ALLOC && sym->sh_flags & SHF_WRITE)
        res = 'B';
    else if (sym->st_shndx == SHN_COMMON)
        res = 'C';
    else if (sym->sh_type & SHT_PROGBITS && sym->sh_flags & SHF_ALLOC)
        res = 'R';

    /* GLOBAL check */
    if (sym->st_bind == STB_LOCAL && res != '?')
        res = (char) ft_tolower(res);
    return res;
}

size_t readSymbolTable(symbol_t *symbols, file_details_t *details) {
    size_t addedSymbols = 0;

    for (size_t i = 0; i < details->table_det.symbols_number; i++) {
        symbol_t sym;
        details->table_det.is64bit ? setSymbolDetails64(details, &sym) : setSymbolDetails32(details, &sym);
        if (isLegalSymbolTableType(&details->table_det, &sym)) {
            symbols[addedSymbols++] = sym;
        }
    }
    if (addedSymbols > 0)
        symbolSort(symbols, 0, (int) addedSymbols - 1);
    return addedSymbols;
}

bool isLegalSymbolTableType(const table_details_t *table_det, const symbol_t *sym_det) {
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