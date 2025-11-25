#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

typedef struct {
    uint64_t total_time;
    uint64_t idle_time;
} cpu_status_t;

typedef struct {
    float load_avg;
    long mem_kb;
    float cpu_usage;
    long cpu_tempera;

    cpu_status_t prev_stat,curr_stat;
    struct {
        _Bool load_avg;
        _Bool mem_available;
        _Bool cpu_temp;
        _Bool cpu_usage;
    } valid;

    time_t timestamp;
} system_status_t;


int collect_stat(system_status_t *system_stat);
_Bool get_system_load(system_status_t *system_stat);
_Bool get_available_memory(system_status_t *system_stat);
_Bool collect_cpu_temp(system_status_t *system_stat);
_Bool collect_cpu_usage(system_status_t *system_stat, cpu_status_t *prev, cpu_status_t *curr);
int read_cpu_stats(cpu_status_t *stats);
float calculate_cpu_usage(const cpu_status_t *prev, const cpu_status_t *curr);
#endif