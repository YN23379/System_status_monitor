#include "common.h"
int collect_cpu_temp()
{
    int fd=open("/sys/class/thermal/thermal_zone2/temp",O_RDONLY);
    if(fd==-1)
    {
        perror("Open /sys/class/thermal/ failed");
        return -1;
    }
    char buffer[10];
    ssize_t byteread=read(fd,buffer,sizeof(buffer)-1);
    if(byteread==-1)
    {
        perror(" /sys/class/thermal/ failed");
        return -1;
    }
    long temp;
    sscanf(buffer,"%ld",&temp);
    close(fd);
    return temp;
}

void printf_cpu_temp()
{
    long temp=collect_cpu_temp()/1000;
    if(temp==-1) 
    printf("Please examine stat_cpu_collector.c");
    else printf("CPU Tempreture:%ld C\n",temp);
}
