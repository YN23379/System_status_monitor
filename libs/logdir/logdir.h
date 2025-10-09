#include<stdio.h>
#include<time.h>
#include"logdir.h"
#include <stdarg.h>
#include<stdlib.h>
typedef struct Log_entry {  //定义日志节点，时间，级别（I，日常信息，W，警告信息，E，错误信息，显示级别越来越高）
	time_t timestamp;        
	char level;            //信息等级
	char data[256];         //信息，和指针域
	struct Log_entry* next;
}LogNode,*LogLinklist;

// 全局链表头
extern LogEntry *log_head;

_Bool log_init(void);                    // 初始化日志系统
_Bool log_add(char level, const char *format, ...); // 添加日志
void log_print_all(void);               // 打印所有日志
void log_cleanup(void);                 // 清理日志系统
