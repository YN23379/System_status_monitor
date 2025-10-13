#include<stdio.h>
#include<time.h>
#include <stdarg.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <errno.h>


#define LOG_ROTATE_SIZE (1 * 1024 * 1024) // 1MB 日志文件大小限制
#define MAX_BACKUP_FILES 4
void log_init();                    // 初始化日志系统
void log_add(int level, const char *format, ...); // 添加日志
void log_print_recent(int i);               // 打印所有日志
long int get_file_size(char *file_path);    //获取文件大小
void log_rollover();                 //日志文件轮转