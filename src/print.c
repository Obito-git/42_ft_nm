#include "ft_nm.h"

static bool isCorrectType(const elf_symbol *symbol, enum Display display) {
    if (ft_strlen(symbol->name) == 0 && symbol->addr_val == 0)
        return false;
    if (display == DISPLAY_NORM || display == DISPLAY_ALL)
        return true;
    if (display == DISPLAY_UNDEFINED && ft_strchr("wU?", symbol->nm_type) != NULL)
        return true;
    if (display == DISPLAY_EXTERNAL && ft_strchr("TBUwWDR", symbol->nm_type) != NULL)
        return true;
    return false;
}

static int print_hex_addr(elf_symbol *symbol, bool is64bit) {
    char *hex_addr = ft_convert_base(symbol->addr_val, "0123456789abcdef");
    int max_len = is64bit ? HEX_64BIT_UINT_MAX_LEN : HEX_32BIT_UINT_MAX_LEN;
    max_len -= (int) ft_strlen(hex_addr);
    if (!hex_addr)
        return handle_error_prefix("malloc error!", "convert to hex", EXIT_FAILURE);
    if (symbol->addr_val > 0 || ft_tolower(symbol->nm_type) == 'n' || ft_tolower(symbol->nm_type) == 't') {
        while (max_len-- > 0)
            ft_printf("0");
        ft_printf("%s", hex_addr);
    } else {
        while (max_len-- > -1)
            ft_printf(" ");
    }
    free(hex_addr);
    return EXIT_SUCCESS;
}

static int print_noname_symbols(const symbol_table_info *table_info) {
    for (size_t i = 0; i < table_info->added_noname_count; i++) {
        elf_symbol **symbols = table_info->noname_symb;
        if (isCorrectType(symbols[i], table_info->display)) {
            if (print_hex_addr(symbols[i], table_info->is64bit) == EXIT_FAILURE)
                return EXIT_FAILURE;
            ft_printf(" %c ", symbols[i]->nm_type);
            ft_printf("%s\n", symbols[i]->name);
        }
    }
    return EXIT_SUCCESS;
}

int print_symbols(const symbol_table_info *table_info, const char *filename, bool print_name) {
    if (print_name)
        ft_printf("\n%s:\n", filename);
    if (table_info->sort == SORT_YES)
        if (print_noname_symbols(table_info) == EXIT_FAILURE)
            return EXIT_FAILURE;
    for (size_t i = 0; i < table_info->added_symbol_count; i++) {
        elf_symbol **symbols = table_info->symbols;
        if (isCorrectType(symbols[i], table_info->display)) {
            if (print_hex_addr(symbols[i], table_info->is64bit) == EXIT_FAILURE)
                return EXIT_FAILURE;
            ft_printf(" %c ", symbols[i]->nm_type);
            ft_printf("%s\n", symbols[i]->name);
        }
    }
    if (table_info->sort == SORT_REVERSE)
        if (print_noname_symbols(table_info) == EXIT_FAILURE)
            return EXIT_FAILURE;
    return EXIT_SUCCESS;
}