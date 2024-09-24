
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	int i;

	i = -1;
	while(envp[++i] != NULL)
		printf("%s\n", envp[i]);
	return (1);
}