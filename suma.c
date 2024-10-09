
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "libft/headers/libft.h"

int main(int argc, char **argv)
{
	char	buff_a[2];
	char	buff_b[2];
	int		num_a;
	int		num_b;
	int		resultado;

	buff_a[1] = '\0';
	buff_b[1] = '\0';
	read(STDIN_FILENO, buff_a[0], 1);
	read(STDIN_FILENO, buff_b[0], 1);

	num_a = ft_atoi(buff_a);
	num_b = ft_atoi(buff_b);
	resultado = num_a + num_b;
	printf("%d", resultado);
	return (0);
}