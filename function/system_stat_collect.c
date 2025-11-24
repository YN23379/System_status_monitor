#include "common.h"

int collect_stat(system_status_t *system_stat)
{
    if(!system_stat) return -1;
    
    memset(system_stat, 0, sizeof(system_status_t));
    system_stat->timestamp = time(NULL);
    
    int success_count = 0;
    
    // 采集系统负载
    if(get_system_load(system_stat)) {
        success_count++;
    }
    
    // 采集CPU温度
    if(collect_cpu_temp(system_stat)) {
        success_count++;
    }
    
    // 采集可用内存
    if(get_available(system_stat)) {
        success_count++;
    }
    
    // CPU使用率需要特殊处理，这里先预留
    // if(collect_cpu_usage(system_stat)) {
    //     success_count++;
    // }
    
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
    if (sscanf(buffer, "%f", &system_stat->load_avg) == 1) {
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
    
    while ((entry = readdir(dir)) != NULL && !found) {
        if (strncmp(entry->d_name, "thermal_zone", 12) != 0) 
            continue;
        
        char path[280], type_path[280];
        snprintf(path, sizeof(path), "/sys/class/thermal/%s/temp", entry->d_name);
        snprintf(type_path, sizeof(type_path), "/sys/class/thermal/%s/type", entry->d_name);
        
        // 检查类型
        FILE *type_file = fopen(type_path, "r");
        if (!type_file) continue;
        
        char type[32] = {0};
        fgets(type, sizeof(type), type_file);
        fclose(type_file);
        
        // 判断是否是CPU温度传感器
        if (strstr(type, "x86") || strstr(type, "cpu") || strstr(type, "core")) {
            FILE *temp_file = fopen(path, "r");
            if (temp_file) {
                int temp;
                if (fscanf(temp_file, "%d", &temp) == 1) {
                    temp /= 1000;  // 转换为摄氏度
                    if (temp > 0 && temp < 120) {
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

_Bool get_available(system_status_t *system_stat)
{
    int fd = open("/proc/meminfo", O_RDONLY);
    if (fd == -1) return 0;
    
    char buffer[1024];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes <= 0) return 0;
    
    buffer[bytes] = '\0';
    char *line = strstr(buffer, "MemAvailable");
    if (line != NULL) {
        long mem_kb;
        // 简化匹配逻辑
        if (sscanf(line, "MemAvailable: %ld kB", &mem_kb) == 1) {
            system_stat->mem_kb = mem_kb;
            system_stat->valid.mem_available = 1;
            return 1;
        }
    }
    
    return 0;
}