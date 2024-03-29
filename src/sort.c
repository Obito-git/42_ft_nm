#include "ft_nm.h"

static int ft_strcmp_ignore_underscore(const char *s1, const char *s2) {
    int c1;
    int c2;
    int s1_nonalnum = 0;
    int s2_nonalnum = 0;

    if (!s1 && !s2)
        return (0);
    if (!s1 || !s2)
        return (1);
    while (*s1 && *s2) {
        while (*s1 && !ft_isalnum(*s1)) {
            s1++;
            s1_nonalnum++;
        }
        while (*s2 && !ft_isalnum(*s2)) {
            s2++;
            s2_nonalnum++;
        }
        c1 = ft_tolower((int) *s1);
        c2 = ft_tolower((int) *s2);
        if (c2 != c1)
            return (c1 - c2);
        if (*s1)
            ++s1;
        if (*s2)
            ++s2;
    }
    if (*s1 == *s2 && s1_nonalnum != s2_nonalnum) {
        if (s1_nonalnum > s2_nonalnum)
            return -1;
        return 1;
    }
    return *s1 - *s2;
}

void symbolSort(elf_symbol **arr, int low, int high) {
    int pivot, i, j;
    elf_symbol temp;
    if (low < high) {
        pivot = low;
        i = low;
        j = high;
        while (i < j) {
            while (i <= high && (ft_strcmp_ignore_underscore(arr[i]->name, arr[pivot]->name) < 0 ||
                                 (ft_strcmp_ignore_underscore(arr[i]->name, arr[pivot]->name) == 0 &&
                                  arr[i]->addr_val <= arr[pivot]->addr_val)))
                i++;
            while (j >= low && (ft_strcmp_ignore_underscore(arr[j]->name, arr[pivot]->name) > 0 ||
                                (ft_strcmp_ignore_underscore(arr[j]->name, arr[pivot]->name) == 0 &&
                                 arr[j]->addr_val > arr[pivot]->addr_val)))
                j--;
            if (i < j) {
                temp = *arr[i];
                *arr[i] = *arr[j];
                *arr[j] = temp;
            }
        }
        temp = *arr[j];
        *arr[j] = *arr[pivot];
        *arr[pivot] = temp;
        symbolSort(arr, low, j - 1);
        symbolSort(arr, j + 1, high);
    }
}





