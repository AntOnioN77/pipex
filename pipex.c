/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/10/04 17:39:19 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "pipex.h"
#include "libft/headers/libft.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
 #include <sys/wait.h>

//Retorna un array de strings con los directorios de la variable
//de entorno PATH
char	**get_paths(char **envp)
{
	int		i;
	char	**paths;

	i = -1;
	paths = NULL;
	while(envp[++i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(&(envp[i][5]), ':');
			assert(paths != NULL); //////////////prueba
			if(paths == NULL)
				return (NULL);
			break;
		}
	}
	//prueba, borrar:
	if (envp[i] == NULL)
		write(1, "variable PATH no encontrada\n", 28);
	//fin prueba
	return (paths);
}

//LA funcion llamadora debera mostrar el codigo de errno en caso de que esta funcion retorne null
char	*find_path(char **envp, char *command)
{
	int i;
	char *pathname;
	char *tmp;
	char **paths;

	paths = get_paths(envp);
	if (paths == NULL)
		return (NULL);
	i= -1;
	while(paths[++i] != NULL)
	{
		tmp = ft_strjoin( paths[i] , "/");
		if(!tmp)
			return (NULL);
		pathname = ft_strjoin( tmp , command);
		free(tmp);
		if(!pathname)
			return (NULL);
		if(access(pathname, X_OK) == 0)
			return(pathname);
	}
	errno = ENOENT;
	return (NULL);
}


// esta MAL. hay que reservar en el proceso padre para poder liberar memoria aunque execve se ejecute correctamente
int	exec_cmd(int narg, char **argv, char **envp)
{
	char *pathname;
	char **cmdflags;

	cmdflags = ft_split(argv[narg], ' ');
	if (ft_split == NULL)
		return (1);
	pathname = find_path(envp, cmdflags[0]);
	if (pathname == NULL)
	{
		perror("");// MAL deberia ser zsh: command not found: <comando>
		return (1);
	}
	execve(pathname, cmdflags, envp);
	return (1);
}

void create_child(t_pipe pipe_fd, int narg, char **argv, char **envp)
{
	int	pid;
	pid = fork();
	printf("%d", pid);
	if (pid == 0)
	{
		printf("Hijo narg:%d ready!!\n", narg);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exec_cmd(narg, argv, envp);
	}
}

void	free_pipes(t_pipe_set *pipe_set)
{
	int	i;

	if (pipe_set == NULL || pipe_set->pipes == NULL)
        return;
	while (i < pipe_set->amount)
	{
		close(pipe_set->pipes[i][0]);
		close(pipe_set->pipes[i][1]);
	}
	free(pipe_set->pipes);
	pipe_set->amount = 0;
	pipe_set->pipes = NULL;
}

int	create_pipes(t_pipe_set *pipe_set, int argc, char **argv)
{
	int i;
	
	if (ft_strcmp(argv[1], "here_doc") == 0)
		pipe_set->amount = argc -3;
	else
		pipe_set->amount = argc -2;
	pipe_set->pipes = ft_calloc(pipe_set->amount, sizeof(t_pipe));
	if (pipe_set->pipes == NULL)
		return (0);
	i = 0;
	while (i < pipe_set->amount)
	{
		if (pipe(pipe_set->pipes[i]) == -1)
		{
			//LANZAR MENSAJE ERROR
			free_pipes(pipe_set);
			return (-1);
		}
		return (0);
	}

}

int	canalize_io(t_pipe_set *pipe_set, char **argv)
{
	int	tmp_fd;

	open("here_doc", O_TMPFILE)
	if (ft_strcmp(argv[1],"here_doc") == 0)
		dup2(pipe_set->pipes[0][0]);

}

int	main(int argc, char **argv, char **envp)
{
	t_pipe_set	*pipe_set;

	create_pipes(pipe_set, argc, argv); 

	
	close(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);
	create_child();
		close(pipe_fd[1]);
	return(0);
}