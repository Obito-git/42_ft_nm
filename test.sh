#!/bin/bash

if [ $# -lt 1 ]; then
    echo -e "\nUsage: $0 <filename> [valgrind]"
    exit 1
fi

filename=$1
valgrind_cmd=""

if [ $# -ge 2 ] && [ "$2" == "valgrind" ]; then
    valgrind_cmd="valgrind "
fi

# NO ARG
nm "$filename" > nm_output.txt
if $valgrind_cmd ./ft_nm "$filename" > ft_nm_output.txt; then
    if diff nm_output.txt ft_nm_output.txt >/dev/null ; then
        echo -e "\nDiff without arg ok\n"
    else
        echo -e "\nDIFF KO\n"
        diff nm_output.txt ft_nm_output.txt
    fi
else
    echo -e "\nError running ft_nm\n"
fi

nm -g "$filename" > nm_output.txt
if $valgrind_cmd ./ft_nm -g "$filename" > ft_nm_output.txt; then
    if diff nm_output.txt ft_nm_output.txt >/dev/null ; then
        echo -e "\nDiff -g (Display only external symbols) ok\n"
    else
        echo -e "\nDIFF -g KO\n"
        diff nm_output.txt ft_nm_output.txt
    fi
else
    echo -e "\nError running ft_nm -g\n"
fi

nm -u "$filename" > nm_output.txt
if $valgrind_cmd ./ft_nm -u "$filename" > ft_nm_output.txt; then
    if diff nm_output.txt ft_nm_output.txt >/dev/null ; then
        echo -e "\nDiff -u (Display only undefined symbols) ok\n"
    else
        echo -e "\nDIFF -u KO\n"
        diff nm_output.txt ft_nm_output.txt
    fi
else
    echo -e "\nError running ft_nm -u\n"
fi

nm -r "$filename" > nm_output.txt
if $valgrind_cmd ./ft_nm -r "$filename" > ft_nm_output.txt; then
    if diff nm_output.txt ft_nm_output.txt >/dev/null ; then
        echo -e "\nDiff -r (Reverse sort) ok\n"
    else
        echo -e "\nDIFF -r KO\n"
        diff nm_output.txt ft_nm_output.txt
    fi
else
    echo -e "\nError running ft_nm -r\n"
fi

nm -p "$filename" > nm_output.txt
if $valgrind_cmd ./ft_nm -p "$filename" > ft_nm_output.txt; then
    if diff nm_output.txt ft_nm_output.txt >/dev/null ; then
        echo -e "\nDiff -p (No sort) ok\n"
    else
        echo -e "\nDIFF -p KO\n"
        diff nm_output.txt ft_nm_output.txt
    fi
else
    echo -e "\nError running ft_nm -p\n"
fi

rm nm_output.txt ft_nm_output.txt
