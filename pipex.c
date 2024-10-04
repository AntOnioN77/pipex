/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:12:18 by antofern          #+#    #+#             */
/*   Updated: 2024/10/04 10:46:27 by antofern         ###   ########.fr       */
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
/*
int run_cmd(t_args *args, int narg, t_fds io)
{
	char	**cmd_and_flags;
	
	cmd_and_flags = ft_split(args->v, " ");
	if (cmd_and_flags == NULL)
		return(ERROR);
}

//el struct io contiene como io.in el fd del archivo de entrada
//y en io.out el fd del archivo de salida, el modo (APPEND o REPLACE)
//se gestiono prebiamente al llamar a open()
void secuence_cmds(int amount, t_args *args, t_fds io)
{
	int		i;
	char	**paths;

	paths = get_paths(args->envp, cmd_and_flags[0]);
	i = 1;
	actual_pat = select_path();
	while(i < args.c)
	{
		run_cmd(args, io);
	}
}
*/
/*
	pathname = find_path(envp, cmdflags[0]);
	if (pathname == NULL)
	{
		perror("");
		return (1);
	}
	execve(pathname, cmdflags, envp);
*/

int	exec_cmd(int narg, char **argv, char **envp)
{
	char *pathname;
	char **cmdflags;

	cmdflags = ft_split(argv[narg], ' ');
	pathname = find_path(envp, cmdflags[0]);
	if (pathname == NULL)
	{
		perror("");// MAL deberia ser zsh: command not found: <comando>
		return (1);
	}
	execve(pathname, cmdflags, envp);
	return (1);
}


/*
int	main(int argc, char **argv, char **envp)
{
	int pid;
	int	i;
	int pipefd[2];
	int file_in;
	int file_out;

	file_out = open(argv[1], O_RDONLY);
	pipe(pipefd);
	dup2(file_in, pipefd[0]);
	i = 1;
	while (argv[++i + 1])
	{
		pid = fork();
		pipe(pipefd);
		dup2(file_in, pipefd[0]);
		dup2(pipefd[1], STDIN_FILENO);
		close(file_in);
		if (pid == 0)
			exec_cmd(argc, argv, envp);
	}

}
*/

int	main(int argc, char **argv, char **envp)
{
	int	pipe_fd[2];
	char buf[356];
	int	pid;
	int	fd;
	int	status;
	int bytesread;

	fd = open("file1", O_RDONLY);
	pipe(pipe_fd);
	
	close(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close(fd);

	pid = fork();
	printf("%d", pid);
	if (pid == 0)
	{
		printf("Hijo ready!!\n");
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exec_cmd(1, argv, envp);
	}
	else
	{
		close(pipe_fd[1]);
		bytesread = 1;
		while(bytesread > 0)
		{
			bytesread = read(pipe_fd[0], buf, 355);
			printf("bytesread: %d\n", bytesread);
			buf[bytesread] = '\0';
			printf("ok: %s ok\n", buf);
		}
	}
	return(0);
}