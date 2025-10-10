#include"common.h"
#include"Memavailable_collector.h"
#include"Loadaverage_collector.h"
#include"stat_cpu_collector.h"
#include"cpu_Tempreture_collector.h"


int main()
{
  //初始数据采集：
  long temp=collect_cpu_temp()/1000;   //CPU温度（C)
  float load =get_system_load();       //负载（1分钟之内系统负载）
  long mem_kb=get_available();         //可用内存（KB）
  float cpu_usage=0.0;                 //CPU使用率（%）
  cpu_status_t prev_stats={0}, curr_stats={0};

  if(load==-1.0)
  printf("Please examine Loadaverage.c\n");
  if(temp==-1) 
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
  	printf("-----System Status(%d)-----\n",i);
	  printf("System Load: %f \n",load);
	  printf("MemAvailable:%ld kB \n",mem_kb);
    printf("CPU usage is:%.2f%%\n",cpu_usage);
    if(temp!=-1||(temp<120&&temp>0) )          //加后面这个是为了再虚拟机上测试
    printf("CPU Tempreture:%ld C\n",temp);

	  prev_stats = curr_stats;
    printf("---------------------------\n");
	  i++;
	  sleep(4);
    temp=collect_cpu_temp()/1000;   //更新CPU温度（C)
    load =get_system_load();        //更新负载
    mem_kb=get_available();         //更新可用内存（KB）
  }
  return 0;
}



