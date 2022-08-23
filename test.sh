#!/bin/sh

gcc -Wall -Werror -Wextra -o pipex pipex.c libft.a && ./pipex infile "tr a b" "tr b c" "tr c d" "tr d e" "tr e f" "tr f g" "tr g h" "tr h i" "tr i j" "tr j k" outfile && echo "-----" && cat -e outfile && rm outfile pipex && touch outfile
