#include "ft_nm.h"

static int ft_strcmp_ignore_underscore(const char *s1, const char *s2) {
    const char *s1_mod;
    const char *s2_mod;
    size_t i;
    for (i = 0; s1[i] == '_'; i++) ;
    s1_mod = &s1[i];
    for (i = 0; s2[i] == '_'; i++) ;
    s2_mod = &s2[i];
    return ft_strcmp_ignore_case(s1_mod, s2_mod);
}

void symbolSort(elf_symbol **arr, int low, int high, enum Sort sort) {
    int pivot, i, j;
    elf_symbol temp;
    if (sort != SORT_NO && low < high) {
        pivot = low;
        i = low;
        j = high;
        while (i < j) {
            if (sort == SORT_YES) {
                while (i <= high && ft_strcmp_ignore_underscore(arr[i]->name, arr[pivot]->name) <= 0)
                    i++;
                while (j >= low && ft_strcmp_ignore_underscore(arr[j]->name, arr[pivot]->name) > 0)
                    j--;
            } else {
                while (i <= high && ft_strcmp_ignore_underscore(arr[i]->name, arr[pivot]->name) >= 0)
                    i++;
                while (j >= low && ft_strcmp_ignore_underscore(arr[j]->name, arr[pivot]->name) < 0)
                    j--;
            }
            if (i < j) {
                temp = *arr[i];
                *arr[i] = *arr[j];
                *arr[j] = temp;
            }
        }
        temp = *arr[j];
        *arr[j] = *arr[pivot];
        *arr[pivot] = temp;
        symbolSort(arr, low, j - 1, sort);
        symbolSort(arr, j + 1, high, sort);
    }
}

void print_symbols(const symbol_table_info *table_info, enum Display display) {
    for (size_t i = 0; i < table_info->added_symbol_count; i++) {
        elf_symbol **symbols = table_info->symbols;
        if (symbols[i]->addr_val > 0) {
            printf("%016lx %c %s\n", symbols[i]->addr_val,
                   symbols[i]->nm_type,
                   symbols[i]->name);
        } else {
            printf("%16s %c %s\n", "",
                   symbols[i]->nm_type,
                   symbols[i]->name);
        }
    }
    (void) display;
}

static char type_by_sym(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx) {
    if (symbol_bind == STB_GNU_UNIQUE)
        return 'u';
    if (symbol_shndx == SHN_ABS)
        return 'A';
    if (symbol_shndx == SHN_UNDEF)
        return 'U';
    if (symbol_shndx == SHN_COMMON)
        return 'C';
    (void ) symbol_type;
    return '?';
}

char symbol_nm_type(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx, uint32_t section_type,
                    uint32_t section_flags) {
    (void) symbol_bind;
    (void) symbol_type;
    (void) symbol_shndx;
    (void) section_type;
    (void) section_flags;
    return type_by_sym(symbol_bind, symbol_type, symbol_shndx);
}