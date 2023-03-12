#include "ft_nm.h"

int handle_error(const char *msg, int error_code) {
    ft_fprintf(STDERR_FILENO, "%s%s", NAME, msg);
    return error_code;
}

int handle_error_prefix(const char *msg, const char *prefixMsg, int error_code) {
    ft_fprintf(STDERR_FILENO, "%s\'%s\': %s", NAME, prefixMsg, msg);
    return error_code;
}
