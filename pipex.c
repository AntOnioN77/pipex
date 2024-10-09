/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/10/09 16:45:12 by antofern         ###   ########.fr       */
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

int	get_current(t_pipe_set  *pipe_set)
{
	return (pipe_set->current);
}

void	dup2_warp(int new_fd, int old_fd)
{
	if (dup2(new_fd, old_fd) == -1)
	{
		perror("dup2 error");
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
		if(pipe_set->current == 0)
		{
			if (argv[1] == "here_doc")
			{
				aux = open(".", O_TMPFILE | S_IRWXU | S_IRWXG);
				dup2_warp(aux, STDIN_FILENO);
			}
			else
			{
				aux = open(argv[1], O_RDONLY);
				dup2_warp(aux, STDIN_FILENO);
			}
		}
		else
		{
			aux = pipe_set->pipes[pipe_set->current -1][0];
			dup2_warp(aux, STDIN_FILENO);
		}
		
		if (dup2(pipe_set->pipes[pipe_set->current][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 error");
			exit(1);
		}
		close(pipe_set->pipes[pipe_set->current][0]);
		close(pipe_set->pipes[pipe_set->current][1]);
		pipe_set->current++;
		exec_cmd(narg, argv, envp);
		return (1); //solo se ejecuta si execve falla;
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
	
	pipe_set->current = 0;
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

/*
int	canalize_io(t_pipe_set *pipe_set, char **argv)
{
	int	tmp_fd;

	open("here_doc", O_TMPFILE)
	if (ft_strcmp(argv[1],"here_doc") == 0)
		dup2(pipe_set->pipes[0][0]);

}
*/

int	canalize_cmd(t_pipe_set pipe_set, char **argv, char **envp)
{
	//create _child()
	wihle ()

}

int	main(int argc, char **argv, char **envp)
{
	t_pipe_set	*pipe_set;
	int	narg;

	create_pipes(pipe_set, argc, argv); 
	narg = 2;
	if (ft_strcmp(argv[1], "here_doc") == 0)
		narg++;
	while (pipe_set->current < pipe_set->amount) //current empieza en 0 entonces while debe terminar en (amount -1)
	{
			create_child(pipe_set, narg, argv, envp);
			close(pipe_set->pipes[pipe_set->current][1]);
			narg++;
	}
	return(0);
}