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
    printf("file size %ldl\n", details.buf.st_size);
    printf("file fd %d\n", details.fd);

    fillHeadersAndSymbolTable(&details);

    Elf64_Sym *sym = details.table_det.symtab;
    for (size_t i = 0; i < details.table_det.symsize / sizeof(Elf64_Sym); i++, sym++) {
        if (ELF64_ST_TYPE(sym->st_info) == STT_FUNC || ELF64_ST_TYPE(sym->st_info) == STT_OBJECT) {
            printf("  %03zu:\t%016lx\t%s\t\t%s\n", i, sym->st_value,
                   ELF64_ST_TYPE(sym->st_info) == STT_FUNC ? "FUNC" : "OBJECT",
                   details.table_det.strtab + sym->st_name);
        }
    }



    if (details.file_start != NULL)
        munmap(details.file_start, details.buf.st_size);
    if (details.fd > 2)
        close(details.fd);
    return EXIT_SUCCESS;
}