#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"cpu_monitor.h"
#include<unistd.h>
int read_cpu_stats(cpu_status_t *stats)
{
  printf("DEBUG: Attempting to open /proc/stat\n");
  FILE *fp=fopen("/proc/stat","r");
  if(!fp)
  {
   perror("Cannot open /proc/stat");
   return -1;
  }
  printf("DEBUG: File opened successfully\n");
  char line[256];
  while(fgets(line, sizeof(line), fp))
  {
   if (strncmp(line,"cpu ",4)==0)
   {
   	unsigned long user, nice, system, idle, iowait, irq, softirq, steal;
	int matched = sscanf(line + 4, "%lu %lu %lu %lu %lu %lu %lu %lu",
		&user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
	if (matched >= 4) 
		{
                fclose(fp);
                // 计算总时间和空闲时间
                stats->total_time = user + nice + system + idle + iowait + irq + softirq + steal;
                stats->idle_time = idle + iowait;
                return 0;
        }
   }


  }
  fclose(fp);
  return -1;
  
}

float calculate_cpu_usage(const cpu_status_t *prev,const cpu_status_t *curr)
{
  if (curr->total_time <= prev->total_time) 
  	{
        return 0.0f;
    }
    
    unsigned long total_delta = curr->total_time - prev->total_time;
    unsigned long idle_delta = curr->idle_time - prev->idle_time;
    
    // 避免除零错误
    if (total_delta == 0) 
	{
        return 0.0f;
    }
    
    return 100.0f * (1.0f - (float)idle_delta / (float)total_delta);
}
void print_cpu_usage(const cpu_status_t *prev, const cpu_status_t *curr) 
{
    float usage = calculate_cpu_usage(prev, curr);
    printf("CPU Usage: %.1f%%\n", usage);
}


