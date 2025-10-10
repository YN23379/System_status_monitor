
#include"logdir.h"


typedef struct Log_entry {  //定义日志节点，时间，级别（I，日常信息，W，警告信息，E，错误信息，显示级别越来越高）
	time_t timestamp;        
	char level;            //信息等级
	char message[256];         //信息，和指针域
	struct Log_entry* next;
}LogNode,*LogLinklist;


LogLinklist L_head = NULL;
LogLinklist L_tail = NULL;


_Bool log_init(LogLinklist L_head)
{
    L_head = NULL;
    L_tail=NULL;
    return 0;
}


_Bool log_add(char level, const char *format, ...)
{
   

}

