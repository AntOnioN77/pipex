
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int	pipe_fd[2];
	char buf[356];

	pipe(pipe_fd);
	write(pipe_fd[1], "eco!", 5);
	read(pipe_fd[0], buf, 5);

	printf("%s\n", buf);
	return(0);
}