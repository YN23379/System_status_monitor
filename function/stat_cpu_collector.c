#include"common.h"
#include"stat_cpu_collector.h"

int read_cpu_stats(cpu_status_t *stats)
{
    static int first_failure = 1;
    int fd=open("/proc/stat",O_RDONLY);;//FILE *fp = fopen("/proc/stat", "r");
    if(fd==-1) //if (!fp) { perror("open /proc/stat"); return -1; }
    {
        if(first_failure)
        {
            perror("Open /proc/stat failed");
            first_failure = 0;
        }
        close(fd);
        return -1;
    } 
    char buffer[512];
    ssize_t byteread=read(fd,buffer,sizeof(buffer)-1);
    if(byteread==-1)
    {
        if(first_failure)
        {
            perror("Read /proc/stat failed");
            first_failure = 0;
        }
        close(fd);
        return -1;
    }
    int flag=-1;
    buffer[byteread]='\0';
    char *line=strstr(buffer,"cpu ");
    if(line!=NULL)
    {
        uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
            // 注意：要求至少8个字段
        sscanf(line+4, "%lu %lu %lu %lu %lu %lu %lu %lu",
                           &user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal);
        stats->idle_time  = idle + iowait;
        stats->total_time = user + nice + system + idle + iowait + irq + softirq + steal;
        flag=1;
    }
    /*while (fgets(line, sizeof line, fp)) 
    {
        if (strncmp(line, "cpu ", 4) == 0) 
        {
            uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
            // 注意：要求至少8个字段
            int m = sscanf(line + 4, "%lu %lu %lu %lu %lu %lu %lu %lu",
                           &user,&nice,&system,&idle,&iowait,&irq,&softirq,&steal);
            if (m < 8) { flag = -1; break; }
            stats->idle_time  = idle + iowait;
            stats->total_time = user + nice + system + idle + iowait + irq + softirq + steal;
            flag = 1;
            break;
        }
    }*/
    close(fd);
    return flag;
}

float calculate_cpu_usage(const cpu_status_t *prev, const cpu_status_t *curr)
{
    if (curr->total_time <= prev->total_time) return -1.0;  //溢出检查
    uint64_t total_delta = curr->total_time - prev->total_time;
    uint64_t idle_delta  = curr->idle_time  - prev->idle_time;
    if (total_delta == 0) return -1.0;
    return 100.f * (1.f - (float)idle_delta / (float)total_delta);
}

 