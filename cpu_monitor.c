#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define CPU_LINE 100
char a[200];
int get_cpu_monitor()  
{                         
    FILE* fp;
    fp = fopen("/proc/stat", "r");
    char file[CPU_LINE];
    if (!fp)
    {
        perror("Cannot open : ");
        return -1;
    }

    else
    {
        while (fgets(file, CPU_LINE, fp) != NULL)
        {
            char* p = strstr(file, "cpu");
            if (p == NULL)
                continue;
            if (strncmp(file, "cpu", 3) == 0 && file[3] == ' ')
                strcpy(a, file);
            return 1;
        }
    }

}

void printf_cpu_monitor()
{
    if (get_cpu_monitor())
    {
        puts(a);         
    }
}
void printf_cpu_usage()
{
    long num[10] = { 0 };
    char jug[2] = " ";
    char* token = strtok(a, jug);
    int i = 0;
    while (token != NULL && i < 10)
    {
        token = strtok(NULL, jug);
        if (token != NULL)
        {
            num[i] = strtol(token, NULL, 10);  
            i++;
        }
    }
    static int total_old = 0, idle_old = 0;
    double usage;
    int total_new = 0, idle_new = 0;
    int total_delta = 0, idle_delta = 0;
    total_old = total_new;
    idle_old = idle_new;
    for (i = 0; i < 10; i++)
    {
        total_new = total_new + num[i];
        if (i == 3 || i == 4)
            idle_new += num[i];
    }
    total_delta = total_new - total_old;
    idle_delta = idle_new - idle_old;

    usage = 1 - (double)idle_delta / total_delta;

    printf("CPU usage is:%.2lf%%\n", usage * 100);
}
void printf_cpu()
{
    printf_cpu_monitor();
    printf_cpu_usage();
}

