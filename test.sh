#!/bin/sh

gcc -Wall -Werror -Wextra pipex.c libft.a && ./a.out infile "tr 'a' 'b'" "tr 'b' 'c'" "tr 'c' 'd'" outfile && echo "-----" && cat -e outfile && rm outfile && touch outfile
