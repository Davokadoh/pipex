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

void	ft_free(char *str)
{
	if (str)
		free(str);
}

void	free_all(char **new_av, char *path)
{
	free_tab(new_av);
	ft_free(path);
}

int	free_all_error(char **new_av, char *path, int error_code)
{
	free_all(new_av, path);
	return (error_code);
}

int	path_error(char **paths, int i)
{
	if (!paths[i])
	{
		perror(paths[i]);
		free_tab(paths);
		return (1);
	}
	return (0);
}

char	*get_path(char *program_name, char **envp)
{
	int		i;
	char	*path;
	char	**paths;

	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			paths = ft_split(envp[i] + 5, ':');
	}
	i = -1;
	while (paths[++i])
	{
		paths[i] = ft_append(paths[i], "/");
		paths[i] = ft_append(paths[i], program_name);
		if (access(paths[i], F_OK | X_OK) == 0)
			break ;
	}
	if (path_error(paths, i))
		return (NULL);
	path = ft_strdup(paths[i]);
	free_tab(paths);
	return (path);
}

int	execute(char *path, char **new_av, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
		execve(path, new_av, envp);
	wait(0);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int		i;
	char	*path;
	char	**new_av;

	i = 0;
	while (++i < ac)
	{
		new_av = ft_split(av[i], ' ');
		if (!new_av || !*new_av || !**new_av) //is that ok ?
			return (free_all_error(new_av, path, -1));
		path = get_path(new_av[0], envp);
		if (!path || !*path)
			return (free_all_error(new_av, path, -2));
		execute(path, new_av, envp);
		free_all(new_av, path);
	}
}
