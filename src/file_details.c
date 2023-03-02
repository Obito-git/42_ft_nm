#include "ft_nm.h"

/*
 * Function is filling file_details_t structure
 * @argument filename is name of file
 * @argument details is struct defined in ft_nm.h
 * is where we will save file fd, file stat and pointer in file start
 * @return errno if an error occurred otherwise returns 0
 */
int parseFileToDetails(char *filename, file_details_t *details) {
    if (details == NULL)
        return EINVAL;
    if ((details->fd = open(filename, O_RDONLY)) < 0)
        return handle_error_prefix(ERRMSG_OPEN_FILE, filename, errno);

    if ((fstat(details->fd, &details->buf)) < 0)
        return handle_error(ERRMSG_FSTAT, errno);

    if (S_ISDIR(details->buf.st_mode))
        return handle_error_prefix(ERRMSG_IS_DIR, filename, EISDIR);

    if (details->buf.st_size < MIN_ELF_BYTES)
        return handle_error_prefix(ERRMSG_NOTELF, filename, EXIT_FAILURE);

    if ((details->file_start = mmap(0,
                                    details->buf.st_size,
                                    PROT_READ,
                                    MAP_PRIVATE,
                                    details->fd,
                                    0)) == MAP_FAILED) {
        return handle_error(ERRMSG_MMAP, errno);
    }
    return 0;
}

void fillSymbolTable32(file_details_t *details) {
    table_details_t *table_det = &details->table_det;
    Elf32_Shdr *symbHdr32 = table_det->symbHdr32;
    for (int i = 0; i < details->ehdr32->e_shnum; i++) {
        if (symbHdr32[i].sh_type == SHT_SYMTAB) {
            table_det->symtab = details->file_start + symbHdr32[i].sh_offset;
            table_det->strsize = symbHdr32[i].sh_size;
            table_det->strtab = (char *) (details->file_start + symbHdr32[symbHdr32[i].sh_link].sh_offset);
            table_det->strsize = symbHdr32[symbHdr32[i].sh_link].sh_size;
            break;
        }
        //FIXME error_handling
    }
}

void fillSymbolTable64(file_details_t *details) {
    table_details_t *table_det = &details->table_det;
    Elf64_Shdr *symbHdr64 = table_det->symbHdr64;
    for (int i = 0; i < details->ehdr64->e_shnum; i++) {
        if (symbHdr64[i].sh_type == SHT_SYMTAB) {
            table_det->symtab = details->file_start + symbHdr64[i].sh_offset;
            table_det->strsize = symbHdr64[i].sh_size;
            table_det->strtab = (char *) (details->file_start + symbHdr64[symbHdr64[i].sh_link].sh_offset);
            table_det->strsize = symbHdr64[symbHdr64[i].sh_link].sh_size;
            break;
        }
        //FIXME error_handling
    }
}

void fillHeadersAndSymbolTable(file_details_t *details) {
    char *ptr = (char *) details->file_start;
    if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
        && ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3) {

        details->ehdr32 = (Elf32_Ehdr *) ptr;
        details->ehdr64 = (Elf64_Ehdr *) ptr;

        if (details->ehdr32->e_ident[EI_CLASS] == ELFCLASS32) {
            details->table_det.symbHdr32 = (Elf32_Shdr *) (ptr + details->ehdr32->e_shoff);
            fillSymbolTable32(details);
            ft_printf("32 bit elf binary\n"); //FIXME delete
        } else if (details->ehdr64->e_ident[EI_CLASS] == ELFCLASS64) {
            details->table_det.symbHdr64 = (Elf64_Shdr *) (ptr + details->ehdr64->e_shoff);
            fillSymbolTable64(details);
            ft_printf("64 bit elf binary\n"); //FIXME delete
        } else {
            ft_printf("unknown elf class\n"); //FIXME handle error
        }
    } else {
        ft_printf("Not elf"); //FIXME handle error
    }
}