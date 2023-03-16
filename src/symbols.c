#include "ft_nm.h"

static int ft_strcmp_ignore_underscore(const char *s1, const char *s2) {
    int c1 = 0;
    int c2 = 0;

    if (!ft_strcmp("__do_global_dtors_aux_fini_array_entry", s1) || !ft_strcmp("__do_global_dtors_aux_fini_array_entry", s2))
        ft_printf("");

    if (!s1 && !s2)
        return (0);
    if (!s1 || !s2)
        return (1);
    while (*s1 && *s2)
    {
        while (!ft_isalnum(*s1))
            s1++;
        while (!ft_isalnum(*s2))
            s2++;
        c1 = ft_tolower((int) *s1);
        c2 = ft_tolower((int) *s2);
        if (c2 != c1)
            return (c1 - c2);
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
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
                while (i <= high && ft_strcmp_ignore_underscore(arr[pivot]->name, arr[i]->name) < 0)
                    i++;
                while (j > low && ft_strcmp_ignore_underscore(arr[pivot]->name, arr[j]->name) >= 0)
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

static bool isCorrectType(const elf_symbol *symbol, enum Display display) {
    if (display == DISPLAY_NORM || display == DISPLAY_ALL)
        return true;
    if (display == DISPLAY_UNDEFINED && ft_strchr("wU?", symbol->nm_type) != NULL)
        return true;
    if (display == DISPLAY_EXTERNAL && ft_strchr("TBUwWDR", symbol->nm_type) != NULL)
        return true;
    return false;
}

int print_symbols(const symbol_table_info *table_info, enum Display display) {
    for (size_t i = 0; i < table_info->added_symbol_count; i++) {
        elf_symbol **symbols = table_info->symbols;
        if (isCorrectType(symbols[i], display)) {
            char *hex_addr = ft_convert_base(symbols[i]->addr_val, "0123456789abcdef");
            int max_len = table_info->is64bit ? HEX_64BIT_UINT_MAX_LEN : HEX_32BIT_UINT_MAX_LEN;
            max_len -= (int) ft_strlen(hex_addr);
            if (!hex_addr)
                return handle_error_prefix("malloc error!", "convert to hex", EXIT_FAILURE);
            if (symbols[i]->addr_val > 0 || ft_tolower(symbols[i]->nm_type) == 'n' || ft_tolower(symbols[i]->nm_type) == 't') {
                while (max_len-- > 0)
                    ft_printf("0");
                ft_printf("%s", hex_addr);
            } else {
                while (max_len-- > -1)
                    ft_printf(" ");
            }
            ft_printf(" %c ", symbols[i]->nm_type);
            ft_printf("%s\n", symbols[i]->name);
            free(hex_addr);
        }
    }
    return EXIT_SUCCESS;
}

static char type_by_section(uint32_t section_type, uint32_t section_flags) {
    if (section_type == SHT_NOBITS && section_flags & SHF_IA_64_SHORT)
        return 'S';
    else if (section_type == SHT_NOBITS && section_flags & (SHF_ALLOC | SHF_WRITE))
        return 'B';
    if (section_type == SHT_IA_64_UNWIND)
        return 'p';
    if ((section_flags & (SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)) == SHF_ALLOC)
        return 'R';
    if ((section_flags & (SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)) == (SHF_ALLOC | SHF_WRITE))
        return (section_flags & SHF_IA_64_SHORT) ? 'G' : 'D';
    if (section_flags == 0)
        return 'N';
    if ((section_flags & (SHF_ALLOC | SHF_EXECINSTR | SHF_WRITE)) == (SHF_ALLOC | SHF_EXECINSTR))
        return 'T';
    return '?';
}

static char type_by_sym(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx) {
    if (symbol_shndx == SHN_ABS)
        return 'A';
    if (symbol_shndx == SHN_COMMON)
        return 'C';
    if (symbol_type == STT_GNU_IFUNC)
        return 'i';
    if (symbol_bind == STB_WEAK) {
        if (symbol_type == STT_OBJECT)
            return symbol_shndx == SHN_UNDEF ? 'v' : 'V';
        else
            return symbol_shndx == SHN_UNDEF ? 'w' : 'W';
    }
    if (symbol_bind == STB_GNU_UNIQUE)
        return 'u';
    if (symbol_shndx == SHN_UNDEF)
        return 'U';
    return '?';
}

char symbol_nm_type(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx, uint32_t section_type,
                    uint32_t section_flags) {
    char res = type_by_sym(symbol_bind, symbol_type, symbol_shndx);
    if (res == '?')
        res = type_by_section(section_type, section_flags);

    if (ft_strchr("?wWvVuUiIp", (int) res) != NULL)
        return res;
    if (symbol_bind == STB_LOCAL)
        res = (char) ft_tolower(res);
    return res;
}