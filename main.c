#include<stdio.h>
#include"Lx_grepMemavailable.h"
#include"Lx_Loadaverage.h"
#include <stdlib.h>
#include <string.h>
#include<unistd.h>


int main()
{
  while(1)
  {
  	printf("-----System Status-----\n");
	print_load_info();
	print_memory_info();
    printf("-----System Status-----\n");
	sleep(5);
  }
  return 0;


}
