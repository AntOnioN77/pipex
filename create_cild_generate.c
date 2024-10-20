#include "pipex.h"
#define _GNU_SOURCE
#include <errno.h>
#include "libft/headers/libft.h"
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


//version de create child, no usar, generada.
int create_child(t_pipe_set *pipe_set, int narg, int argc, char **argv, char **envp)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        return (1);
    }
    if (pid == 0)
    {
        // Código del proceso hijo

        // Si no es el primer comando, redirigimos la entrada
        if (pipe_set->current > 0)
        {
            // Redirigimos stdin al extremo de lectura del pipe anterior
            dup2_warp(pipe_set->pipes[pipe_set->current - 1][0], STDIN_FILENO);
        }
        else
        {
            // Primer comando: entrada desde archivo
            int fd_in = open(argv[1], O_RDONLY);
            if (fd_in == -1)
            {
                perror("Error opening input file");
                exit(1);
            }
            dup2_warp(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // Si no es el último comando, redirigimos la salida
        if (pipe_set->current < pipe_set->amount - 1)
        {
            // Redirigimos stdout al extremo de escritura del pipe actual
            dup2_warp(pipe_set->pipes[pipe_set->current][1], STDOUT_FILENO);
        }
        else
        {
            // Último comando: salida a archivo
            int fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out == -1)
            {
                perror("Error opening output file");
                exit(1);
            }
            dup2_warp(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        // Cerrar todos los pipes no utilizados
        for (int i = 0; i < pipe_set->amount; i++)
        {
            close(pipe_set->pipes[i][0]);
            close(pipe_set->pipes[i][1]);
        }

        // Ejecutar el comando
        exec_cmd(narg, argv, envp);
        perror("exec_cmd error");
        exit(1);
    }
    else
    {
        // Código del proceso padre

        // Cerramos los extremos que ya no necesitamos
        if (pipe_set->current > 0)
        {
            close(pipe_set->pipes[pipe_set->current - 1][0]);
            close(pipe_set->pipes[pipe_set->current - 1][1]);
        }

        // Incrementamos el contador
        pipe_set->current++;
    }

    return (0);
}
