#include<stdio.h>
#include<time.h>
#include <stdarg.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>


void log_init();                    // 初始化日志系统
void log_add(int level, const char *format, ...); // 添加日志
void log_print_recent(int i);               // 打印所有日志
void log_clean();             // 清理日志系统
void log_print_recent(int i);