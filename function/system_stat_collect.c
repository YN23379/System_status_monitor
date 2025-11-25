#include "common.h"

int collect_stat(system_status_t *system_stat)
{
    if(!system_stat) return -1;
    
    memset(system_stat, 0, sizeof(system_status_t));
    system_stat->timestamp = time(NULL);
    
    int success_count = 0;
    
    // 采集系统负载
    if(get_system_load(system_stat)) 
    {
        success_count++;
    }
    
    // 采集CPU温度
    if(collect_cpu_temp(system_stat)) 
    {
        success_count++;
    }
    
    // 采集可用内存
    if(get_available_memory(system_stat)) 
    {
        system_stat->valid.cpu_usage=1;
        success_count++;
    }
    
    // CPU使用率
    system_stat->prev_stat=system_stat->curr_stat;
    read_cpu_stats(&system_stat->curr_stat);
    if(system_stat->cpu_usage=calculate_cpu_usage(&system_stat->prev_stat,&system_stat->curr_stat)) 
    {
        success_count++;
    }
    
    return success_count; // 返回成功采集的指标数量
}

_Bool get_system_load(system_status_t *system_stat)
{
    int fd = open("/proc/loadavg", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[60];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes <= 0) return 0;
    
    buffer[bytes] = '\0';
    if (sscanf(buffer, "%f", &system_stat->load_avg) == 1) 
    {
        system_stat->valid.load_avg = 1;
        return 1;
    }
    
    return 0;
}

_Bool collect_cpu_temp(system_status_t *system_stat)
{
    DIR *dir = opendir("/sys/class/thermal");
    if (!dir) return 0;
    
    struct dirent *entry;
    _Bool found = 0;
    
    while ((entry = readdir(dir)) != NULL && !found) 
    {
        if (strncmp(entry->d_name, "thermal_zone", 12) != 0) 
            continue;
        
        char path[280], type_path[280];
        snprintf(path, sizeof(path), "/sys/class/thermal/%s/temp", entry->d_name);
        snprintf(type_path, sizeof(type_path), "/sys/class/thermal/%s/type", entry->d_name);
        

        FILE *type_file = fopen(type_path, "r");
        if (!type_file) continue;
        
        char type[32] = {0};
        fgets(type, sizeof(type), type_file);
        fclose(type_file);
        
        if (strstr(type, "x86") || strstr(type, "cpu") || strstr(type, "core")) 
        {
            FILE *temp_file = fopen(path, "r");
            if (temp_file) 
            {
                int temp;
                if (fscanf(temp_file, "%d", &temp) == 1) 
                {
                    temp /= 1000;  // 转换为摄氏度
                    if (temp > 0 && temp < 120) 
                    {
                        system_stat->cpu_tempera = temp;
                        system_stat->valid.cpu_temp = 1;
                        found = 1;
                    }
                }
                fclose(temp_file);
            }
        }
    }
    
    closedir(dir);
    return found;
}

_Bool get_available_memory(system_status_t *system_stat)
{
    int fd = open("/proc/meminfo", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[1024];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes <= 0) return 0;
    
    buffer[bytes] = '\0';
    char *line = strstr(buffer, "MemAvailable");
    if (line != NULL) 
    {
        long mem_kb;
        if (sscanf(line, "MemAvailable: %ld kB", &mem_kb) == 1) 
        {
            system_stat->mem_kb = mem_kb;
            system_stat->valid.mem_available = 1;
            return 1;
        }
    }
    
    return 0;
}

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
