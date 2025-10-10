#include"common.h"
#include"Loadaverage_collector.h"

float get_system_load() {
    static int first_failure = 1;
    int  fd = open("/proc/loadavg", O_RDONLY); //读取文件
    //FILE *fp=fopen("/proc/loadavg","r");
    if(fd==-1)
    {
      if(first_failure)
      {
        perror("Open /proc/loadavg failed");
        first_failure=0;
      }
      close(fd);
      return -1.0;
    }
    //if(!fp) return -1.0;
    float load;
    char buffer[60];
    ssize_t flag=read(fd,buffer, sizeof(buffer) - 1); //留个空给'\0'
    //float load'fscanf(file, "%f", &load);
    if(flag==-1)
    {
      if(first_failure)
      {
        perror("Read /proc/loadavg failed");
        first_failure=0;
      }
      close(fd);            
      return -1.0;
    }
    buffer[flag]='\0';//确保能形成字符串
    
    if(sscanf(buffer,"%f",&load)!=1)
    {
      if(first_failure)
      {
        perror("Sscanf /proc/loadavg failed");
        first_failure=0;
      }
      close(fd);             //fclose(file);
      return -1.0;
    }
    close(fd);             
    return load;
}

