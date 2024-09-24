
#include <fcntl.h>
 #include <unistd.h>
#include <stdio.h>

int main(void)
{
	int	pid;
	int fd;
	int	pid2;

	pid = fork();
	pid2 = fork();
	printf("pid = %d pid2 = %d \n", pid, pid2);
	if(pid == 0)
		fd = open("./proceso hijo", O_WRONLY | O_APPEND | O_CREAT, 00744);
	else
		fd = open("./proceso padre", O_WRONLY | O_APPEND | O_CREAT, 00744);
	write(fd, "prueba", 6);
	close(fd);
	return (0);
}