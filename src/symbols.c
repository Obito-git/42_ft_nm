#include "ft_nm.h"

elf_symbol *elf_constructor(const char *name, uint64_t st_value, char type) {
    elf_symbol *result = (elf_symbol *) malloc(sizeof(elf_symbol));
    if (!result)
        return NULL;
    result->name = name;
    result->addr_val = st_value;
    result->nm_type = type;
    return result;
}

static char get_type_by_section(uint32_t section_type, uint32_t section_flags) {
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

static char get_type_by_sym(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx) {
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

char
get_nm_symbol_type(unsigned char symbol_bind, unsigned char symbol_type, uint16_t symbol_shndx, uint32_t section_type,
                   uint32_t section_flags) {
    char res = get_type_by_sym(symbol_bind, symbol_type, symbol_shndx);
    if (res == '?')
        res = get_type_by_section(section_type, section_flags);

    if (ft_strchr("?wWvVuUiIpN", (int) res) != NULL)
        return res;
    if (symbol_bind == STB_LOCAL)
        res = (char) ft_tolower(res);
    return res;
}