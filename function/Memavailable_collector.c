#include"common.h"
#include"Memavailable_collector.h"
long get_memory_available() 
{
    static int first_failure = 1;
    int fd =open("/proc/meminfo",O_RDONLY);//FILE* file = fopen("/proc/meminfo", "r");
    if(fd==-1)//if (!file) return -1;
    {
        if(first_failure)
        {
            perror("Open /proc/meminfo failed");
            close(fd);
            first_failure=0;
        }
        return -1;
    }
    /*下面不可行，porc是虚拟文件，不支持lseek操作 
    // 获取文件大小
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek meminfo failed");
        close(fd);
        return -1;
    }
    lseek(fd, 0, SEEK_SET); // 重置文件指针*/
    
    char buffer[1024]={0};//char line[256];
    ssize_t byteread=read(fd,buffer,sizeof(buffer)-1);
    if(byteread==-1)
    {
         if(first_failure)
        {
            perror("Read /proc/meminfo failed");
            close(fd);
            first_failure=0;
        }
        return -1;
    }

    //buffer[byteread]='\0';
    //需要用额外的line记录strstr返回的指针不能直接：
    // if(strstr(buffer,"MemAvailable")!=NULL),这样之后的解析就定位不到MemAvailable，会从文件开头解析了
    
    buffer[byteread] = '\0';
    char *line = strstr(buffer, "MemAvailable");
    long mem_kb =-1;
    if (line != NULL) 
    {
        if (sscanf(line, "MemAvailable: %ld kB", &mem_kb) != 1||sscanf(line, "MemAvailable:%ld kB", &mem_kb) != 1) //匹配格式更全面
         mem_kb = -1;
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
    return mem_kb;
}
void print_memory_info()
{
  long mem_kb=get_memory_available();
  if(mem_kb!=-1)
  	printf("MemAvailable:%ld kB \n",mem_kb);
  else
  {
    printf("Please examine Memavaliable.c\n");
  }
}



