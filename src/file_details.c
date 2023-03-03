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

/* 32bit
 * Parsing symbol table details and filling table_details_t
 *
 * @argument details is struct defined in ft_nm.h
 * @argument secNumb elf header section number
 * @argument symbHdr32 is pointer on 32bit section header
 * @return //FIXME add return (error case)
 */
void fillSymbolTable32(file_details_t *details, uint16_t secNumb, Elf32_Shdr *symbHdr32) {
    table_details_t *table_det = &details->table_det;
    for (int i = 0; i < secNumb; i++) {
        if (symbHdr32[i].sh_type == SHT_SYMTAB) {
            table_det->symtab = details->file_start + symbHdr32[i].sh_offset;
            table_det->symsize = symbHdr32[i].sh_size;
            table_det->strtab = (char *) (details->file_start + symbHdr32[symbHdr32[i].sh_link].sh_offset);
            //table_det->strsize = symbHdr32[symbHdr32[i].sh_link].sh_size;
            break;
        }
        //FIXME error_handling
    }
}

/* 64bit
 * Parsing symbol table details and filling table_details_t
 *
 * @argument details is struct defined in ft_nm.h
 * @argument secNumb elf header section number
 * @argument symbHdr64 is pointer on 64bit section header
 * @return //FIXME add return (error case)
 */
void fillSymbolTable64(file_details_t *details, uint16_t secNumb, Elf64_Shdr *symbHdr64) {
    table_details_t *table_det = &details->table_det;
    for (int i = 0; i < secNumb; i++) {
        if (symbHdr64[i].sh_type == SHT_SYMTAB) {
            table_det->symtab = details->file_start + symbHdr64[i].sh_offset;
            table_det->symsize = symbHdr64[i].sh_size;
            table_det->strtab = (char *) (details->file_start + symbHdr64[symbHdr64[i].sh_link].sh_offset);
            //table_det->strsize = symbHdr64[symbHdr64[i].sh_link].sh_size;
            break;
        }
        //FIXME error_handling
    }
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
void fillHeadersAndSymbolTable(file_details_t *details) {
    char *ptr = (char *) details->file_start;
    if (ptr[0] == ELFMAG0 && ptr[1] == ELFMAG1
        && ptr[2] == ELFMAG2 && ptr[3] == ELFMAG3) {

        Elf32_Ehdr *ehdr32 = (Elf32_Ehdr *) ptr;
        Elf64_Ehdr *ehdr64 = (Elf64_Ehdr *) ptr;

        if (ehdr32->e_ident[EI_CLASS] == ELFCLASS32) {
            fillSymbolTable32(details, ehdr32->e_shnum, (Elf32_Shdr *) (ptr + ehdr32->e_shoff));
            details->table_det.is64bit = false;
            ft_printf("32 bit elf binary\n"); //FIXME delete
        } else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64) {
            fillSymbolTable64(details, ehdr64->e_shnum, (Elf64_Shdr *) (ptr + ehdr64->e_shoff));
            details->table_det.is64bit = true;
            ft_printf("64 bit elf binary\n"); //FIXME delete
        } else {
            ft_printf("unknown elf class\n"); //FIXME handle error
        }
    } else {
        ft_printf("Not elf"); //FIXME handle error
    }
}