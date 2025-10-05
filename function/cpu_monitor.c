#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include"cpu_monitor.h"
int read_cpu_stats(cpu_status_t *stats)
{
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) { perror("open /proc/stat"); return -1; }

    char line[512];
    int ok = -1;

    while (fgets(line, sizeof line, fp)) 
    {
        if (strncmp(line, "cpu ", 4) == 0) 
        {
            uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
            // 注意：要求至少8个字段
            int m = sscanf(line + 4, "%lu %lu %lu %lu %lu %lu %lu %lu",
                           &user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal);
            if (m < 8) { ok = -1; break; }
            stats->idle_time  = idle + iowait;
            stats->total_time = user + nice + system + idle + iowait + irq + softirq + steal;
            ok = 1;
            break;
        }
    }
    fclose(fp);
    return ok;
}

float calculate_cpu_usage(const cpu_status_t *prev, const cpu_status_t *curr)
{
    if (curr->total_time <= prev->total_time) return 0.f;
    uint64_t total_delta = curr->total_time - prev->total_time;
    uint64_t idle_delta  = curr->idle_time  - prev->idle_time;
    if (total_delta == 0) return 0.f;
    return 100.f * (1.f - (float)idle_delta / (float)total_delta);
}
void print_cpu_usage(const cpu_status_t *prev, const cpu_status_t *curr)
{
  float cpu_usage=0.0;
  cpu_usage=calculate_cpu_usage(prev,curr);
  printf("CPU usage is:%.2f%%\n",cpu_usage);
}


 