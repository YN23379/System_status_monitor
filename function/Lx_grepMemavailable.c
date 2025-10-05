#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lx_grepMemavailable.h"
long get_memory_available() {
    FILE* file = fopen("/proc/meminfo", "r");
    if (!file) return -1;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "MemAvailable")) {
            long mem_kb;
            sscanf(line, " MemAvailable: %ld kB ", &mem_kb);
            fclose(file);
            return mem_kb;  }
    }
    fclose(file);
    return -1;
}
void print_memory_info()
{
  int memory=get_memory_available();
  if(memory!=-1)
  	printf("MemAvailable:%1d KB \n",memory);
}



