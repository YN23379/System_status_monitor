#include"common.h"
#include"Memavailable_collector.h"
long get_memory_available() {
    int fd =open("/proc/meminfo",O_RDONLY);//FILE* file = fopen("/proc/meminfo", "r");
    if(fd==-1)//if (!file) return -1;
    {
        perror("Open /proc/meminfo failed");
        return -1;
    }
    char buffer[1024]={0};//char line[256];
    ssize_t byteread=read(fd,buffer,sizeof(buffer) - 1);
    if(byteread==-1)
    {
        perror("Read /proc/meminfo failed");
        close(fd);
        return -1;
    }
    buffer[byteread]='\0';
    //需要用额外的line记录strstr返回的指针不能直接：
    // if(strstr(buffer,"MemAvailable")!=NULL),这样之后的解析就定位不到MemAvailable，会从文件开头解析了
    char *line = strstr(buffer, "MemAvailable");
    if (line != NULL) {
        long mem_kb = 0;
        if (sscanf(line, "MemAvailable: %ld kB", &mem_kb) == 1||sscanf(line, "MemAvailable:%ld kB", &mem_kb) == 1) 
        {
            close(fd);
            return mem_kb;  
        }
    }
    /* while (fgets(line, sizeof(line), file)) 
    {
        if (strstr(line, "MemAvailable")) 
        {
            long mem_kb;
            sscanf(line, " MemAvailable: %ld kB ", &mem_kb);
            fclose(file);
            return mem_kb;  
        }
    }*/
    
    close(fd);//fclose(file);
    return -1;
}
void print_memory_info()
{
  long mem_kb=get_memory_available();
  if(mem_kb!=-1)
  	printf("MemAvailable:%ld kB \n",mem_kb);
  else
  {
    printf("Cannot printf MemAvailabe correctly\n");
  }
}



