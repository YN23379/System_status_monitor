#include<stdio.h>
#include<time.h>
#include <stdarg.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
typedef struct Log_entry {  //定义日志节点，时间，级别（I，日常信息，W，警告信息，E，错误信息，显示级别越来越高）
	time_t timestamp;   
	void *udata;     
	char level;            //信息等级
	char message[256];         //信息，和指针域
	struct Log_entry* next;
}LogNode,*LogLinklist;

// 全局链表头
extern LogNode *log_head;

void log_init();                    // 初始化日志系统
void log_add(int level, const char *format, ...); // 添加日志
void log_print_recent(int i);               // 打印所有日志
void log_clean();             // 清理日志系统
void log_print_recent(int i);