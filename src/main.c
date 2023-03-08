#include "ft_nm.h"

/*
 * Function is filling file_details_t structure
 * @argument filename is name of file
 * @argument details is struct defined in ft_nm.h
 * is where we will save file fd, file stat and pointer in file start
 * @return errno if an error occurred otherwise returns 0
 */
static int parseFileToDetails(char *filename, file_details_t *details) {
    if (details == NULL)
        return EINVAL;
    if ((details->fd = open(filename, O_RDONLY)) < 0)
        return handle_error_prefix(ERRMSG_OPEN_FILE, filename, errno);

    if ((fstat(details->fd, &details->file_stat)) < 0)
        return handle_error(ERRMSG_FSTAT, errno);

    if (S_ISDIR(details->file_stat.st_mode))
        return handle_error_prefix(ERRMSG_IS_DIR, filename, EISDIR);

    if (details->file_stat.st_size < MIN_ELF_BYTES)
        return handle_error_prefix(ERRMSG_NOTELF, filename, EXIT_FAILURE);

    if ((details->file_start = mmap(0,
                                    details->file_stat.st_size,
                                    PROT_READ,
                                    MAP_PRIVATE,
                                    details->fd,
                                    0)) == MAP_FAILED) {
        return handle_error(ERRMSG_MMAP, errno);
    }
    return 0;
}

/* Checks file content and is it an elf file.
 * Then checks architecture and calls filling symbol table details 32/64 function to fill details :D
 * @argument details is struct defined in ft_nm.h
 *
 * @vartype Elf32/64_Ehdr = Elf file header
 * @vartype ehdr.e_ident contains magic number array of type uchar
 * @vartype ehdr.e_shoff is section header table file offset
 * @vartype ehdr.>e_shnum is sections number
 *
 * @return //FIXME add ret
 */
static void fillHeadersAndSymbolTable(file_details_t *details) {
    char *ptr = (char *) details->file_start;
    if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
        && ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3) {

        Elf32_Ehdr *ehdr32 = details->ehdr32 = (Elf32_Ehdr *) ptr;
        Elf64_Ehdr *ehdr64 = details->ehdr64 = (Elf64_Ehdr *) ptr;

        details->shdr32 = (Elf32_Shdr *) (ptr + ehdr32->e_shoff);
        details->shdr64 = (Elf64_Shdr *) (ptr + ehdr64->e_shoff);

        if (ehdr32->e_ident[EI_CLASS] == ELFCLASS32) {
            fillSymbolTable32(details);
            details->table_det.is64bit = false;
            ft_printf("32 bit elf binary\n"); //FIXME delete
        } else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64) {
            fillSymbolTable64(details);
            details->table_det.is64bit = true;
            ft_printf("64 bit elf binary\n"); //FIXME delete
        } else {
            ft_printf("unknown elf class\n"); //FIXME handle error
        }
    } else {
        ft_printf("Not elf"); //FIXME handle error
    }
}

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
    /*
    printf("addr %p\n", details.file_start);
    printf("file size %ldl\n", details.file_stat.st_size);
    printf("file fd %d\n", details.fd);
    */

    fillHeadersAndSymbolTable(&details);

    symbol_t symbols[details.table_det.symbols_number];
    size_t addedSymbols = readSymbolTable(symbols, &details);


    for (size_t i = 0; i < addedSymbols; i++) {
        printf("  %03zu:\t%016lx\t%c\t\t%-40s\n", i, symbols[i].st_value,
               get_symbol_type(&symbols[i]),
               details.table_det.strtab + symbols[i].st_name);
    }


    //print_details(&details);

    for (size_t i = 0; i < addedSymbols; i++)
        free(symbols[i].symbol_name);
    if (details.file_start != NULL)
        munmap(details.file_start, details.file_stat.st_size);
    if (details.fd > 2)
        close(details.fd);
    return EXIT_SUCCESS;
}