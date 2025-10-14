#include<stdio.h>
#include<time.h>
#include <stdarg.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <errno.h>
#include<sys/file.h>

#define MAXLOGDIR 10
#define LOG_ROTATE_SIZE (1 * 1024 * 1024) // 1MB 日志文件大小限制
#define MAX_BACKUP_FILES 4
#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_FATAL 2
#define TOTAL_MEMORY 1*1024*8124 //总内存 KB
void log_init();                    // 初始化日志系统
void log_add(int level,const char *format, ...); // 添加日志
void log_print_recent(int i);               // 打印倒数i行日志
long int get_file_size(char *file_path);    //获取文件大小
void log_rollover();                 //日志文件轮转
int determine_monitor_level(float cpu_usage, float load, long temp, long mem_available); //日志等级判定 