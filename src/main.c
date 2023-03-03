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
    print_details(&details);




    if (details.file_start != NULL)
        munmap(details.file_start, details.file_stat.st_size);
    if (details.fd > 2)
        close(details.fd);
    return EXIT_SUCCESS;
}