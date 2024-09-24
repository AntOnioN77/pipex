
#include <unistd.h>
#include <stdio.h>

int	main(void)
{
	if (access("/usr/bin/ls", X_OK) == 0)
		printf("ls es ejecutable");
	else
		printf("ls no es ejecutable");
	return (0);
}
