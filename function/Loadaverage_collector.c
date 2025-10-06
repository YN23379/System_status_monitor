#include"common.h"
#include"Loadaverage_collector.h"
float string_to_float(const char *str) //解析文本，把字符数字转换成浮点类型
{
    float result = 0.0;
    float decimal_place = 0.1;

    // 处理整数部分
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0'); // 将字符转为数字并累加
        str++;
    }

    // 处理小数部分
    if (*str == '.') {
        str++;
        while (*str >= '0' && *str <= '9') {
            result += (*str - '0') * decimal_place;
            decimal_place /= 10;
            str++;
        }
    }

    return result;
}
float get_system_load() {
    
    int  fd = open("/proc/loadavg", O_RDONLY); //读取文件
    //FILE *fp=fopen("/proc/loadavg","r");
    if(fd==-1)
    {
      perror("Open /proc/loadavg failed");
      return -1.0;
    }
    //if(!fp) return -1.0;
    
    char buffer[60];
    float load;
    ssize_t flag=read(fd,buffer, sizeof(buffer) - 1); //留个空给'\0'
    //float load'fscanf(file, "%f", &load);
    if(flag==-1)
    {
      perror("Read /proc/loadavg failed");
      return -1.0;
    }
    buffer[flag]='\0';//确保能形成字符串
    close(fd);             //fclose(file);
    if(sscanf(buffer,"%f",&load)!=1)
    {
      perror("Sscanf buffer failed");
      return -1.0;
    }
    return load;
}

void print_load_info()
{
  
  float load =get_system_load();
  if(load!=-1.0)
  printf("System Load: %f \n",load);
}



