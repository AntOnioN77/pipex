/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/10/13 21:04:28 by antofern         ###   ########.fr       */
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
	errno = ENOENT;// posiblemente prohibido
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

void	dup2_warp(int old_fd, int new_fd)
{

	if (dup2(old_fd, new_fd) == -1)
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
	close(pipe_set->pipes[pipe_set->current][0]);
	if (aux == -1)
	{
		perror("fork error");
		return (1);
	}
	if (aux == 0)
	{
printf("136---child says:\n");
fflush(stdout);
		if(pipe_set->current == 0)
		{
printf("140 primera iteracion.\n");
fflush(stdout);
			if (ft_strcmp(argv[1], "here_doc") == 0)
			{
printf("144 abriendo: here_doc\n");
fflush(stdout);
				aux = open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);

				dup2_warp(aux, STDIN_FILENO);
			}
			else
			{
printf("153 abriendo %s\n", argv[1]);
fflush(stdout);
				aux = open(argv[1], O_RDONLY);
printf("156 duplicando %s en stdin\n", argv[1]);
fflush(stdout);
				dup2_warp(aux, STDIN_FILENO);
			}
		}
		else
		{
printf("162 pipe_current: %d duplicando STDIN(%d) en fd%d \n", pipe_set->current, STDIN_FILENO, pipe_set->pipes[pipe_set->current -1][0]);
fflush(stdout);
			aux = pipe_set->pipes[pipe_set->current -1][0];
			dup2_warp(STDIN_FILENO, aux);
		}
		if ((pipe_set->current + 1) < pipe_set->amount)//posiblemente siempre se cumple
		{
printf("amount%d \n", pipe_set->amount);
fflush(stdout);
printf("169 duplicando extremo de escritura del pipe current(%d) (fd%d)\n", pipe_set->pipes[pipe_set->current][1], pipe_set->pipes[pipe_set->current][1]);
fflush(stdout);
			dup2_warp(pipe_set->pipes[pipe_set->current][1], STDOUT_FILENO);
//			close_both(pipe_set->pipes[pipe_set->current]);
			close(pipe_set->pipes[pipe_set->current][1]);
		}
		perror("175");
		exec_cmd(narg, argv, envp);
		perror("177");
		return (1); //solo se ejecuta si execve falla;
	}
	pipe_set->current++;
waitpid(aux, NULL, 0);// solo usar en depuracion
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
		amount = argc -4;
	else
		amount = argc -3;
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
	int outfile = open(argv[argc - 1], O_WRONLY); //DE PRUEBA //falta O_CREAT...
	dup2_warp(outfile, STDOUT_FILENO);
	while ((pipe_set.current) < pipe_set.amount)
	{											// current se incrementa dentro de create_child
printf("238 create_child\n");
fflush(stdout);
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