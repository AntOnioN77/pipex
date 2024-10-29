#ifndef TRACE_TOOLS
#define TRACE_TOOLS

//Imprime un mensaje en LOG.txt con informacion de archivo linea y parametros
//RECOMENDADO usar macro LOG_MESSAGE() 
void log_message(const char *level, const char *process_name, int process_id, int pipe_id, const char *message, const char *file_name, int line_number);

/*Genera un identificador mediante una variable estatica (1 ... 2 ... 3 ...) en caso de usarse
para identificar procesos hijos, debe situarse antes de llamar a fork()*/
int	get_id(void);

/* Dibuja un marcador de fin de programa al final del archivo LOG.txt*/
int log_end();

//Lee de el pipe o archivo <fd_read>, escribe lo leido en LOG.txt y en <fd_write>
void monitor_pipe(int fd_read, int fd_write);

/*	-Imprime contenido del archivo asociado a <fd>, maximo <sample_len> 99 
despues de la lectura devuelve el offset a la posicion original.
No sirve para leer en pipes*/
void	test_sample_fd(int fd, char *expected, int sample_len);

# define LOG_MESSAGE(level, process_name, process_id, pipe_id, message) \
    log_message(level, process_name, process_id, pipe_id, message, __FILE__, __LINE__)

#endif