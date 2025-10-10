#include "common.h"
int collect_cpu_temp()
{
    static int first_failure = 1;
    int fd=open("/sys/class/thermal/thermal_zone2/temp",O_RDONLY);
    if(fd==-1)
    {
        if(first_failure)
        {
            perror("Open /sys/class/thermal/ failed");
            first_failure = 0;
        }
        close(fd);
        return -1;
    }
    char buffer[10];
    ssize_t byteread=read(fd,buffer,sizeof(buffer)-1);
    if(byteread==-1)
    { 
        if(first_failure)
        {
            perror("Read /sys/class/thermal/ failed");
            first_failure = 0;
        }
        close(fd);
        return -1;
    }
    long temp=0;
    sscanf(buffer,"%ld",&temp);
    close(fd);
    return temp;
}
