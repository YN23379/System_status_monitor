#include "common.h"

int main() 
{
    system_status_t stat;

    //采集cpu时间片基准
    read_cpu_stats(&stat.curr_stat);
    
    while(1) 
    {
        sleep(1);
        int collected = collect_stat(&stat);
        read_cpu_stats(&stat.curr_stat);
        
        printf("采集到 %d 个指标:\n", collected);
        if(stat.valid.load_avg) 
        {
            printf("系统负载: %.2f\n", stat.load_avg);
        }
        if(stat.valid.cpu_temp) 
        {
            printf("CPU温度: %ld°C\n", stat.cpu_tempera);
        }
        if(stat.valid.mem_available) 
        {
            printf("可用内存: %ld KB\n", stat.mem_kb);
        }
        if(stat.valid.cpu_usage)
        {
            printf("CPU使用率:%.2f%%\n",stat.cpu_usage);
        }
        sleep(1);
    }
    
    return 0;
}