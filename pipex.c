/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/10/30 00:51:05 by antofern         ###   ########.fr       */
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

#include "trace_tools/trace_tools.h"


/* 	-FUNCION DE prueba. separar del codigo fuente original y archivar en libreria "test.h"
	-Imprime contenido del archivo asociado a <fd>, maximo <sample_len> 99 */
void	test_sample_fd(int fd, char *expected, int sample_len)
{
	char	test[100];
	off_t	original_offset;
	ssize_t	bytes_read;

    if (sample_len >= 100)
	{
        fprintf(stderr, "test_sample_fd Error: el tamaño de sample_len es superior al limite para esta función (99)\n");
        return;
    }
	if (fd < 0)
	{
		perror("test_sample_fd, valor en fd no valido ");
		fflush(stderr);
        return;
	}
	original_offset = lseek(fd, 0, SEEK_CUR);
    if (original_offset == -1)
	{
        perror("test_sample_fd, error al obtener el offset inicial");
		fflush(stderr);
        return;
    }
	test[sample_len] = '\0';

    bytes_read = read(fd, test, sample_len);
    if (bytes_read == -1)
	{
        perror("Error al leer el file descriptor");
        return;
    }
	lseek(fd, original_offset, SEEK_SET);
	printf("In fd:%d EXPECTED:\n %s \n YOURS:\n %s\n", STDIN_FILENO, expected, test);
	fflush(stdout);
}

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
	//TO DO: No solo notificar, tambien liberar lo que sea preciso
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

int id = get_id();//Borrar
LOG_MESSAGE("INFO", "Padre", id, -42, "Forking");
	aux = fork();

	if (aux == -1)
	{
		perror("fork error");
		return (1);
	}
	if (aux == 0)
	{
LOG_MESSAGE("INFO", "Hijo", id, -42, "Comienza proceso");
		if (pipe_set->current == 0)
		{
LOG_MESSAGE("INFO", "Hijo", id, -42, "current == 0 (primer proceso hijo)");
			if (ft_strcmp(argv[1], "here_doc") == 0)
			{
LOG_MESSAGE("INFO", "Hijo", id, -42, "abriendo HERE_DOC ...");
				aux = open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
				ssize_t bytesread = 1;
				char *buffer[100];
				while (bytesread > 0)
				{
					bytesread = read(0, buffer, 100);
					write(aux, buffer, bytesread);
				}
				dup2_warp(aux, STDIN_FILENO);
LOG_MESSAGE("INFO", "Hijo", id, -42, "file descriptor de here_doc duplicado en STDIN_FILENO (previo stdin queda cerrado)");
//test_sample_fd(STDIN_FILENO, "contenido de here_doc", 3);
			}
			else
			{
LOG_MESSAGE("INFO", "Hijo", id, -42, "abriendo infile");	
				aux = open(argv[1], O_RDONLY);
LOG_MESSAGE("INFO", "Hijo", id, -42, "Duplicando infile en stdin");
				dup2_warp(aux, STDIN_FILENO);
//test_sample_fd(STDIN_FILENO, "contenido de file1", 3);
			}
		}
		else
		{
LOG_MESSAGE("INFO", "Hijo", id, (pipe_set->current - 1), "Duplicando STDIN en estremo de lectura");
			aux = pipe_set->pipes[pipe_set->current - 1][0];
			dup2_warp(aux, STDIN_FILENO);
//test_sample_fd(STDIN_FILENO, "retorno de wc", 3);
		}
		if (pipe_set->current < pipe_set->amount - 1)
		{
LOG_MESSAGE("INFO", "Hijo", id, (pipe_set->current), "Duplicando estremo de escritura del pipe en stdout");

			dup2_warp(pipe_set->pipes[pipe_set->current][1], STDOUT_FILENO);
		}
		perror("175");//??????????????????????
		for (int i = 0; i < pipe_set->amount; i++)//for no se puede
        {
LOG_MESSAGE("INFO", "Hijo", id, i, "Cerrando pipe (ambos extremos)");
            close(pipe_set->pipes[i][0]);
            close(pipe_set->pipes[i][1]);
        }
LOG_MESSAGE("INFO", argv[narg], id, -42, "ejecutando");
		exec_cmd(narg, argv, envp);
		perror("177");
		return (1); //solo se ejecuta si execve falla;
	}
	else
    {
        if (pipe_set->current > 0)
        {
LOG_MESSAGE("INFO", "Padre", -42, pipe_set->current, "Cerrando pipe (ambos extremos)");
            close(pipe_set->pipes[pipe_set->current - 1][0]);
            close(pipe_set->pipes[pipe_set->current - 1][1]);
        }
	   pipe_set->current++;
    }
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
LOG_MESSAGE("INFO", "Padre", -42, -42, "abriendo outfile");
	int outfile = open(argv[argc - 1], O_WRONLY); //DE PRUEBA //falta O_CREAT...
LOG_MESSAGE("INFO", "Padre", -42, -42, "Duplicando outfile en stdout");
	dup2_warp(outfile, STDOUT_FILENO);
	while ((pipe_set.current) < pipe_set.amount)
	{											// current se incrementa dentro de create_child
			if (create_child(&pipe_set, narg, argv, envp) != 0)
			{
				perror("create_child error");// cambiar por printf, la variable errno del proceso hijo no es accesible desde aquí 
				return (1);
			}
			narg++;
	}
	int status;
	for (int i = 0; i < pipe_set.amount; i++)
	{
		wait(&status);
	}
log_end();
	return(0);
}