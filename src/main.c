#include "ft_nm.h"

static int openFile(char *filename, int *fd, void **file_start, struct stat *stat) {
    if ((*fd = open(filename, O_RDONLY)) < 0)
        return handle_error_prefix(ERRMSG_OPEN_FILE, filename, EXIT_FAILURE);

    if ((fstat(*fd, stat)) < 0)
        return handle_error(ERRMSG_FSTAT, EXIT_FAILURE);

    if (S_ISDIR(stat->st_mode))
        return handle_error_prefix(ERRMSG_IS_DIR, filename, EXIT_FAILURE);

    if (stat->st_size < MIN_ELF_BYTES)
        return handle_error_prefix(ERRMSG_NOTELF, filename, EXIT_FAILURE);

    if ((*file_start = mmap(0,
                            stat->st_size,
                            PROT_READ,
                            MAP_PRIVATE,
                            *fd,
                            0)) == MAP_FAILED) {
        return handle_error(ERRMSG_MMAP, EXIT_FAILURE);
    }
    return 0;
}

int main(int ac, char **av) {
    struct stat stat;

    int fd = -1;
    void *file_start = NULL;
    int exit_code = 0;

    if (ac == 1) {
        av[0] = DEFAULT_FILE_NAME;
    } else {
        ac--;
        av = &av[1];
    }
    for (int i = 0; i < ac; i++) {
        if (openFile(av[i], &fd, &file_start, &stat) == 0) {
            if (handleElf(file_start, av[i], ac > 1) == EXIT_FAILURE)
                exit_code++;
            if (fd > 0)
                close(fd);
            if (file_start != NULL)
                munmap(file_start, stat.st_size);
        } else
            exit_code++;
    }
    return exit_code;
}