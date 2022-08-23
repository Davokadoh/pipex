#!/bin/sh

gcc -Wall -Werror -Wextra pipex.c libft.a && ./a.out infile "tr a b" "tr b c" "tr c d" "tr d e" "tr e f" outfile && echo "-----" && cat -e outfile && rm outfile && touch outfile
