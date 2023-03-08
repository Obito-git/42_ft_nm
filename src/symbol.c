#include "ft_nm.h"

static void symbolSort(symbol_t arr[], int low, int high) {
    int pivot, i, j;
    symbol_t temp;
    if (low < high) {
        pivot = low;
        i = low;
        j = high;
        while (i < j) {
            while (ft_strcmp_ignore_case(arr[i].symbol_name, arr[pivot].symbol_name) <= 0  && i <= high) {
                i++;
            }
            while (ft_strcmp_ignore_case(arr[j].symbol_name, arr[pivot].symbol_name) > 0  && j >= low) {
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

size_t readSymbolTable(symbol_t *symbols, file_details_t *details) {
    size_t addedSymbols = 0;

    for (size_t i = 0; i < details->table_det.symbols_number; i++) {
        symbol_t sym;
        details->table_det.is64bit ? setSymbolDetails64(details, &sym) : setSymbolDetails32(details, &sym);
        if (isLegalSymbolTableType(&details->table_det, &sym)) {
            symbols[addedSymbols++] = sym;
        }
    }
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