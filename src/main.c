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

static int process_args(int ac, char **av, char **file_names, enum Sort *sort, enum Display *display) {
    int files_count = 0;

    for (int i = 1; i < ac; i++) {
        size_t arg_len = ft_strlen(av[i]);
        if (av[i][0] == '-' && arg_len > 1) {
            for (size_t y = 1; y < arg_len; y++) {
                if (av[i][y] == 'a') {
                    if (DISPLAY_ALL > *display)
                        *display = DISPLAY_ALL;
                } else if (av[i][y] == 'g') {
                    if (DISPLAY_EXTERNAL > *display)
                        *display = DISPLAY_EXTERNAL;
                } else if (av[i][y] == 'u') {
                    if (DISPLAY_UNDEFINED > *display)
                        *display = DISPLAY_UNDEFINED;
                } else if (av[i][y] == 'r') {
                    if (SORT_REVERSE > *sort)
                        *sort = SORT_REVERSE;
                } else if (av[i][y] == 'p') {
                    if (SORT_NO > *sort)
                        *sort = SORT_NO;
                } else {
                    PRINT_UNKNOWN_ARG_ERROR(av[i][y]);
                    return -1;
                }
            }
        } else
            file_names[files_count++] = av[i];
    }
    return files_count;
}

int main(int ac, char **av) {
    struct stat stat;

    char *file_names[ac];
    enum Sort sort = SORT_YES;
    enum Display display = DISPLAY_NORM;
    int fd = -1;
    void *file_start = NULL;
    int exit_code = 0;

    int files_count = process_args(ac, av, file_names, &sort, &display);

    if (files_count >= 0) {
        if (files_count == 0) {
            files_count++;
            file_names[0] = "a.out";
        }
        for (int i = 0; i < files_count; i++) {
            if (openFile(file_names[i], &fd, &file_start, &stat) == 0) {
                if (handleElf(file_start, file_names[i], files_count > 1, sort, display) == EXIT_FAILURE)
                    exit_code++;
                if (fd > 0)
                    close(fd);
                if (file_start != NULL)
                    munmap(file_start, stat.st_size);
            } else
                exit_code++;
        }
    } else
        exit_code++;
    return exit_code;
}