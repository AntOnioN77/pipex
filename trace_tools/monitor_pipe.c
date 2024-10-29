#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//AVISO: codigo generado automaticamente y sin testear. Es un esbozo, una idea.

//Lee de el pipe o archivo <fd_read>, escribe lo leido en LOG.txt y en <fd_write>
void monitor_pipe(int fd_read, int fd_write)
{
    char buffer[1024];
    ssize_t bytes_read;
    int log_fd = open("LOG.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

    while ((bytes_read = read(fd_read, buffer, sizeof(buffer))) > 0)
	{
        // Escribe en el archivo de log
		write(log_fd, "|| [", 4);
	    write(log_fd, buffer, bytes_read);
		write(log_fd, "]\n", 2);
        // Pasa el contenido al siguiente pipe o stdout
        write(fd_write, buffer, bytes_read);
    }
    close(log_fd);
}