#include "ft_nm.h"

int handle_error(char *msg, int error_code) {
    ft_fprintf(STDERR_FILENO, msg);
    return error_code;
}

int handle_error_prefix(char *msg, char *prefixMsg, int error_code) {
    ft_fprintf(STDERR_FILENO, "%s: %s", prefixMsg, msg);
    return error_code;
}
