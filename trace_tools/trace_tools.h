#ifndef TRACE_TOOLS
#define TRACE_TOOLS
void log_message(const char *level, const char *process_name, int process_id, int pipe_id, const char *message);
int	get_id(void);
#endif