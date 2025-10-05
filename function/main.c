#include<stdio.h>
#include"Lx_grepMemavailable.h"
#include"Lx_Loadaverage.h"
#include"cpu_monitor.h"
#include<unistd.h>
#include<stdint.h>

int main()
{
  
  cpu_status_t prev_stats={0}, curr_stats={0};
  if (read_cpu_stats(&prev_stats) != 1) 
  	{
        fprintf(stderr, "Initial CPU stats read failed\n");
        return 1;
    }
    
  int i=1;
  while(1)
  {
  	sleep(2); //cpu_stats采样间隔
    if (read_cpu_stats(&curr_stats) != 1)
  	{
        fprintf(stderr, "CPU stats read failed\n");
    } 
  	printf("-----System Status(%d)-----\n",i);
	  print_load_info();
	  print_memory_info();
    print_cpu_usage(&prev_stats, &curr_stats);
	  prev_stats = curr_stats;
    printf("---------------------------\n");
	
	i++;
	sleep(3);  //系统采样间隔
  }
  return 0;


}
