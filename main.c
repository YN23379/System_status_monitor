#include<stdio.h>
#include"Lx_grepMemavailable.h"
#include"Lx_Loadaverage.h"
#include"cpu_monitor.h"
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"cpu_monitor.h"

int main()
{
  cpu_status_t prev_stats, curr_stats;
  if (read_cpu_stats(&prev_stats) != 0) 
  	{
        fprintf(stderr, "Initial CPU stats read failed\n");
        return 1;
    }
  if (read_cpu_stats(&curr_stats) != 0)
  	{
        fprintf(stderr, "CPU stats read failed\n");
    }
  int i=1;
  while(1)
  {
  	
  	printf("-----System Status(%d)-----\n",i);
	print_load_info();
	print_memory_info();
    print_cpu_usage(&prev_stats, &curr_stats);
	prev_stats = curr_stats;
    printf("---------------------------\n");
	
	i++;
	sleep(5);
  }
  return 0;


}
