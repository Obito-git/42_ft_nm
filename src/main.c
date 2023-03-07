#include "ft_nm.h"

int main(int ac, char **av) {
    if (ac != 1 && ac != 2) {
        ft_putendl_fd(ERRMSG_ARGS_COUNT, STDERR_FILENO);
        return EXIT_FAILURE;
    }

    char *filename = ac == 1 ? "a.out" : av[1];
    file_details_t details;

    int res = parseFileToDetails(filename, &details);
    if (res != 0)
        return res;
    printf("addr %p\n", details.file_start);
    printf("file size %ldl\n", details.file_stat.st_size);
    printf("file fd %d\n", details.fd);

    fillHeadersAndSymbolTable(&details);

    symbol_t symbols[details.table_det.symbols_number];
    (void) symbols;
    size_t addedSymbols = 0;
    
    for (size_t i = 0; i < details.table_det.symbols_number; i++) {
        symbol_t sym;
        setSymbolDetails(&details, &sym);
        if (isLegalSymbolTableType(&details.table_det, &sym)) {
            symbols[addedSymbols++] = sym;
        }
    }

    symbolSort(symbols, 0, (int) addedSymbols - 1);

        for (size_t i = 0; i < addedSymbols; i++) {
        printf("  %03zu:\t%016lx\t%c\t\t%s\t\t%s\n", i, symbols[i].st_value,
               get_symbol_type(ELF64_ST_TYPE(symbols[i].st_info), ELF64_ST_BIND(symbols[i].st_info),
                               ELF64_ST_VISIBILITY(symbols[i].st_info)),
               details.table_det.strtab + symbols[i].st_name,
               symbols[i].section_name);
    }


    //print_details(&details);


    if (details.file_start != NULL)
        munmap(details.file_start, details.file_stat.st_size);
    if (details.fd > 2)
        close(details.fd);
    return EXIT_SUCCESS;
}