#include "libft.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define READ 0
#define WRITE 1

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
		perror("path error");
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

void	infile(int io_fds[2], int pipes[2][2])
{
		write(2, "Here1\n", 6);
		dup2(io_fds[0], STDIN);
		dup2(pipes[0][WRITE], STDOUT);
}

void	outfile(int i, int io_fds[2], int pipes[2][2])
{
	write(2, "Here4\n", 6);
	if (i % 2)
	{
		close(pipes[0][WRITE]);
		dup2(pipes[0][READ], STDIN);
	}
	else
	{
		close(pipes[1][WRITE]);
		dup2(pipes[1][READ], STDIN);
	}
	dup2(io_fds[1], STDOUT);
}

void	child_pipe_management(int i, int max, int io_fds[2], int pipes[2][2])
{
	if (i == 0)
		infile(io_fds, pipes);
	else if (i == max - 4)
		outfile(i, io_fds, pipes);
	else if (i % 2)
	{
		write(2, "Here2\n", 6);
		dup2(pipes[0][READ], STDIN);
		dup2(pipes[1][WRITE], STDOUT);
	}
	else
	{
		write(2, "Here3\n", 6);
		dup2(pipes[1][READ], STDIN);
		dup2(pipes[0][WRITE], STDOUT);
	}
	write(2, "Executing cmd ", 14);
	write(2, ft_itoa(i), 1);
	write(2, "\n", 2);
}

int	execute(char *path, char **new_av, char **envp, int i, int max, int io_fds[2], int pipes[2][2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		child_pipe_management(i, max, io_fds, pipes);
		execve(path, new_av, envp);
	}
	waitpid(pid, &status, 0);
	if (i % 2)
		close(pipes[1][WRITE]);
	else
		close(pipes[0][WRITE]);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	int		i;
	char	*path;
	char	**new_av;
	int		io_fds[2];
	int		pipes[2][2];

	io_fds[0] = open(av[1], O_RDWR);
	io_fds[1] = open(av[ac - 1], O_RDWR);
	i = -1;
	while (++i < ac - 3)
	{
		if (pipe(pipes[i % 2]) == -1)
			return (3);
		new_av = ft_split(av[i + 2], ' ');
		if (!new_av || !*new_av || !**new_av)
			return (free_all_error(new_av, path, 1));
		path = get_path(new_av[0], envp);
		if (!path || !*path)
			return (free_all_error(new_av, path, 2));
		execute(path, new_av, envp, i, ac, io_fds, pipes);
		free_all(new_av, path);
	}
	printf("End of pipex\n");
	return (0);
}
