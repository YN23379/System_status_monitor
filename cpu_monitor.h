#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H
typedef struct{
 unsigned int total_time;
 unsigned int idle_time;
}cpu_status_t;

int read_cpu_stats(cpu_status_t *stats);
float calculate_cpu_usage(const cpu_status_t *prev,const cpu_status_t *curr);
void print_cpu_usage(const cpu_status_t *prev, const cpu_status_t *curr);

#endif
