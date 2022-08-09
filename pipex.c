#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*ft_append(char *s1, char *s2)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = (char *) malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(*s1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcpy(str + ft_strlen(s1), s2, ft_strlen(s2) + 1);
	free(s1);
	return (str);
}

void	free_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		if (tab[i])
			free(tab[i]);
	free(tab);
}

int	main(int ac, char **av, char **envp)
{
	int		pid;
	int		i;
	char	**paths;
	char	**new_av;

	(void) ac;
	(void) av;

	paths = NULL;
	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			paths = ft_split(envp[i] + 5, ':');
	}
	new_av = ft_split(av[1], ' ');
	i = -1;
	while (paths[++i])
	{
		paths[i] = ft_append(paths[i], "/");
		paths[i] = ft_append(paths[i], new_av[0]);
		if (!access(paths[i], 0))
			break ;
	}
	if (!paths[i])
	{
		perror(paths[i]);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		execve(paths[i], &new_av[0], envp);
	free_tab(new_av);
	free_tab(paths);
}
