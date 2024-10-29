#include <stdio.h>
#include <time.h>

/*SOLO PARA PRUEBAS. Borrar de version para entregar
Cada llamada a esta función escribe una linea en LOG.txt
con level (INFO, DEBUG, ERROR), marca de tiempo, process_name, pipe_id, message.
Usar -42 cuando un id no sea relevante*/
void log_message(const char *level, const char *process_name, int process_id, int pipe_id, const char *message, const char *file_name, int line_number)
{
    time_t now;
    char time_str[20];
    struct tm *time_info;

    // Obtener el tiempo actual
    time(&now);
    time_info = localtime(&now);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);

    // Abrir el archivo LOG.txt en modo append, crear si no existe
    FILE *log_file = fopen("LOG.txt", "a");
    if (log_file == NULL) {
        perror("Error al abrir el archivo LOG.txt");
        return;
    }

    // Escribir en el archivo log
	if(pipe_id == -42)
	    fprintf(log_file, "[%s] [%s] [%s %d] [NoPip] [%s:%d] %s\n", level, time_str, process_name, process_id, file_name, line_number, message);
	else
	    fprintf(log_file, "[%s] [%s] [%s %d] [Pipe %d] [%s:%d] %s\n", level, time_str, process_name, process_id, pipe_id, file_name, line_number, message);


    // Cerrar el archivo
    fclose(log_file);
}

// Definir una macro para simplificar la llamada a log_message
#define LOG_MESSAGE(level, process_name, process_id, pipe_id, message) \
    log_message(level, process_name, process_id, pipe_id, message, __FILE__, __LINE__)

/*
// Ejemplo de uso:
int main() {
    LOG_MESSAGE("INFO", "MainProcess", 1234, 1, "Inicio del proceso principal");
    return 0;
}
*/