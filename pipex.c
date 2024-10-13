/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/10/11 15:37:07 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <errno.h>
#include "pipex.h"
#include "libft/headers/libft.h"
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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
	if (cmdflags == NULL)
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

int	get_current(t_pipe_set  *pipe_set)
{
	return (pipe_set->current);
}

void	dup2_warp(int new_fd, int old_fd)
{
printf("line 108 new_fd==%d\n", new_fd);
fflush(stdout);
	if (dup2(new_fd, old_fd) == -1)
	{
		perror("dup2 error");
		exit(1);
	}
}

void	close_both(t_pipe pipe)
{
	if (close(pipe[0]) == -1 || close(pipe[1]) == -1)
	{
		perror("close error");
		exit(1);
	}
}

int create_child(t_pipe_set *pipe_set, int narg, char **argv, char **envp)
{
	int	aux;

	aux = fork();
	if (aux == -1)
	{
		perror("fork error");
		return (1);
	}
	if (aux == 0)
	{
printf("line 137 current==%d-----------------------\n", pipe_set->current);
fflush(stdout);
		if(pipe_set->current == 0)
		{
			if (ft_strcmp(argv[1], "here_doc") == 0)
			{
				aux = open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
printf("line 144 aux==%d\n", aux);
fflush(stdout);
				dup2_warp(aux, STDIN_FILENO);
			}
			else
			{
				aux = open(argv[1], O_RDONLY);
perror("open error: ");
printf("line 152 aux==%d\n", aux);
fflush(stdout);
				dup2_warp(aux, STDIN_FILENO);
			}
		}
		else
		{
			aux = pipe_set->pipes[pipe_set->current -1][0];
printf("line 161 aux==%d current== %d\n", aux, pipe_set->current);
fflush(stdout);
			dup2_warp(aux, STDIN_FILENO);
		}
		if (pipe_set->current != pipe_set->amount)
		{
			dup2_warp(pipe_set->pipes[pipe_set->current][1], STDOUT_FILENO);
			close_both(pipe_set->pipes[pipe_set->current]);
		}
		exec_cmd(narg, argv, envp);
		return (1); //solo se ejecuta si execve falla;
	}
	pipe_set->current++;
	waitpid(aux, NULL, 0);
	return (0);
}

void	free_pipes(t_pipe_set *pipe_set)
{
	int	i;

	i = 0;
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
	int	amount;
	
	if (ft_strcmp(argv[1], "here_doc") == 0)
		amount = argc -3;
	else
		amount = argc -2;
	pipe_set->pipes = ft_calloc(amount, sizeof(t_pipe));
	if (pipe_set->pipes == NULL)
		return (-1);
	pipe_set->amount = amount;
	pipe_set->current = 0;
	i = 0;
	while (i < pipe_set->amount)
	{
		if (pipe(pipe_set->pipes[i]) == -1)
		{
			//LANZAR MENSAJE ERROR
			free_pipes(pipe_set);
			return (-1);
		}
		i++;
	}
	return (0);
}

/*
int	canalize_io(t_pipe_set *pipe_set, char **argv)
{
	int	tmp_fd;

	open("here_doc", O_TMPFILE)
	if (ft_strcmp(argv[1],"here_doc") == 0)
		dup2(pipe_set->pipes[0][0]);

}
*/

int	main(int argc, char **argv, char **envp)
{
	t_pipe_set	pipe_set;
	int	narg;

	create_pipes(&pipe_set, argc, argv);
	narg = 2;
	if (ft_strcmp(argv[1], "here_doc") == 0)
		narg++;
	while (pipe_set.current <= pipe_set.amount)
	{											// current se incrementa dentro de create_child
			if (create_child(&pipe_set, narg, argv, envp) != 0)
			{
				perror("create_child error");// cambiar por printf, la variable errno del proceso hijo no es accesible desde aquí 
				return (1);
			}
			close(pipe_set.pipes[pipe_set.current][1]);
			narg++;
	}
	return(0);
}