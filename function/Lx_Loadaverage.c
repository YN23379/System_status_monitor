#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lx_Loadaverage.h"
float get_system_load() {
    FILE* file = fopen("/proc/loadavg", "r");
    if (!file) return -1.0;
    
    float load;
    fscanf(file, "%f", &load);
    fclose(file);
    return load;
}

void print_load_info()
{
  float load =get_system_load();
  printf("System Load: %f \n",load);
}



