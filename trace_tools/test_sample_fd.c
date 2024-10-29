
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


/*	-Imprime contenido del archivo asociado a <fd>, maximo <sample_len> 99 */
void	test_sample_fd(int fd, char *expected, int sample_len)
{
	char	test[100];
	off_t	original_offset;
	ssize_t	bytes_read;

    if (sample_len >= 100)
	{
        fprintf(stderr, "test_sample_fd Error: el tamaño de sample_len es superior al limite (99)\n");
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
        perror("Error al leer el archivo");
        return;
    }
	lseek(fd, original_offset, SEEK_SET);
	printf("In fd:%d EXPECTED:\n %s \n YOURS:\n %s\n", STDIN_FILENO, expected, test);
	fflush(stdout);
}