#include"common.h"
#include"Memavailable_collector.h"
#include"Loadaverage_collector.h"
#include"stat_cpu_collector.h"
#include"cpu_Tempreture_collector.h"
#include"logdir.h"

int main()
{
  //初始数据采集：
  long tempera=collect_cpu_temp()/1000;   //CPU温度（C)
  float load =get_system_load();       //负载（1分钟之内系统负载）
  long mem_kb=get_available();         //可用内存（KB）
  float cpu_usage=0.0;                 //CPU使用率（%）
  cpu_status_t prev_stats={0}, curr_stats={0};

  if(load==-1.0)
  printf("Please examine Loadaverage.c\n");
  if(tempera==-1) 
  printf("Please examine cpu_Tempreture_collector.c\n");
  if(mem_kb==-1)
  printf("Please examine Memavaliable.c\n");
  if (read_cpu_stats(&prev_stats) == -1) 
  {
    fprintf(stderr, "Initial CPU stats read failed\n");
    return 1;
  }
  int i=1;           //记录循环次数
  
  while(1)
  {
    
  	sleep(3);    //cpu_stats采样间隔
    if (read_cpu_stats(&curr_stats) != 1)
  	{
      fprintf(stderr, "CPU stats read failed\n");
    } 
    cpu_usage=calculate_cpu_usage(&prev_stats,&curr_stats);   //更新CPU使用率
    int level= determine_monitor_level(cpu_usage,load,tempera,mem_kb);
  	printf("-----System Status(%d)-----\n",i);
	  printf("System Load: %.2f \n",load);
	  printf("MemAvailable:%ld kB \n",mem_kb);
    printf("CPU usage is:%.2f%%\n",cpu_usage);
    if(tempera!=-1&&tempera<120&&tempera>0 )          //加后面这个是为了再虚拟机上测试
    printf("CPU Tempreture:%ld C\n",tempera);
	  prev_stats = curr_stats;
    
    //日志记录
    char *s="CPU usage:%.2f%% , System loadavg:%.2f , CPU temp:%d C , Memavailable:%d KB";
    if(get_file_size("/home/devuser/Desktop/code/System_status_monitor/libs/log")>LOG_ROTATE_SIZE)   //日志文件大小判断，是否轮换
    log_rollover();
	  log_add(level,s,cpu_usage,load,tempera,mem_kb);
    log_print_recent(5);
    printf("---------------------------\n");
	  i++;
	  sleep(2);
    tempera=collect_cpu_temp()/1000;   //更新CPU温度（C)
    load =get_system_load();        //更新负载
    mem_kb=get_available();         //更新可用内存（KB）
  }
  return 0;
}



