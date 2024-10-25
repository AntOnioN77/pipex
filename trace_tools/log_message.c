#include <stdio.h>
#include <time.h>

/*SOLO PARA PRUEBAS. Borrar de version para entregar
Cada llamada a esta función escribe una linea en LOG.txt
con level (INFO, DEBUG, ERROR), marca de tiempo, process_name, pipe_id, message.
Usar -42 cuando un id no sea relevante*/
void log_message(const char *level, const char *process_name, int process_id, int pipe_id, const char *message)
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
    fprintf(log_file, "[%s] [%s] [%s %d] [Pipe %d] %s\n", level, time_str, process_name, process_id, pipe_id, message);

    // Cerrar el archivo
    fclose(log_file);
}