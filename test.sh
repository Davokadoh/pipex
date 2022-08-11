#!/bin/sh

gcc -Wall -Werror -Wextra pipex.c libft.a && ./a.out infile "cat -" "cat - infile" outfile && echo "-----" && cat -e outfile
