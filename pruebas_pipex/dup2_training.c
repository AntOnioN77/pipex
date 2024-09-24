
#include <unistd.h>
#include <fcntl.h>

int	main(void)
{
	int fd;
	
	fd = open("./test.txt", O_WRONLY | O_APPEND);
	dup2(fd, STDOUT_FILENO);
	write(STDOUT_FILENO, "\nimpreso en salida estandar", 19);
	close(fd);
}