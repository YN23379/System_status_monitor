#include"common.h"
#include"Memavailable_collector.h"
#include"Loadaverage_collector.h"
#include"stat_cpu_collector.h"
#include"cpu_Tempreture_collector.h"

int main()
{
  sleep(2);
  cpu_status_t prev_stats={0}, curr_stats={0};
  if (read_cpu_stats(&prev_stats) == -1) 
  	{
        fprintf(stderr, "Initial CPU stats read failed\n");
        return 1;
    }
  if(collect_cpu_temp()==-1)
  {
    printf("Cannot get CPU temp\n");
    return 0;
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
    printf_cpu_temp();

	  prev_stats = curr_stats;
    printf("---------------------------\n");
	  i++;
	  sleep(5);
  }
  return 0;

}
