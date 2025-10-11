#include"logdir.h"
#define MAXLOGDIR 10   //日志链表的最大节点数
/*static struct{         //日志系统内部判断依据L
  int level;
  time_t timeL;
}L;*/

LogLinklist L_head = NULL;       //日志系统头指针
//LogLinklist L_tail = NULL;
static FILE *log_fp=NULL;            // 日志输出文件存放位置
static const char* level_strings[] = {   //level等级
  "INFO", "WARN", "FATAL"
};

void log_init()                   //初始化日志系统
{
    L_head =(LogNode *)malloc(sizeof(LogNode));
	log_fp=fopen("/home/devuser/Desktop/code/System_status_monitor/log","a+");

	if(log_fp==NULL)
	{
		perror("Fopen Log file failed");
		fclose(log_fp);
	}

	if(L_head==NULL) 
	{
		perror("L_head Init failed");
	}
	L_head->next=NULL;
}

char *gettimedata()  //读取当前时间
{
	time_t Nodetime;
	time(&Nodetime);
	struct tm *tm_info=localtime(&Nodetime);
    static char buffer[64];
	strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S",tm_info);
	return buffer;
}

void log_add(int level, const char *format, ...)   //有头结点,创建添加新节点
{
    LogNode *newNode=(LogNode *)malloc(sizeof(LogNode));
    if(newNode==NULL)
    {
	 perror("Create new Node failed");
    }
	//初始化新节点
    
    newNode->level=level;
    newNode->next=NULL;
    //printf("调试前%s\n",format);

	va_list args;                   //可变参数的解析,新节点信息获取
	va_start(args,format);
	vsnprintf(newNode->message,sizeof(newNode->message),format,args);
	va_end(args);
    
	//printf("调试后:%s\n",newNode->message);
 
	newNode->next=L_head->next; 
	L_head->next=newNode;

	if(log_fp!=NULL)   //日志系统文件输出
	{
        char _time[64];
		char *s=_time;
		s=gettimedata();

		fprintf(log_fp,"[%s] %s %s \n",s,level_strings[level],newNode->message);
        fflush(log_fp);  // 立即刷新，确保数据写入磁盘
	}

}
//错误：没有处理空链表的情况，函数声明与实际返回类型不一致，缺少时间获取失败的处理

void log_print_all()
{
	int i=1;
    LogNode *p=L_head->next;
	char buffer[64];
	char *s=buffer;
	s=gettimedata();
    while(p!=NULL&&i<=10)
    {
	 printf("%s 日志节点(%d):\n内容:%s\n",s,i,p->message);
	 i++;
	 p=p->next;
    }
}
/*int main()
{

	log_init();
	int a=111;
    float cpu_usage=0.1134,loadavg=0.01;
	int temp=53,Memavailable=54311230;
    char *s="CPU usage:%.2f%%,System loadavg:%.2f,CPU temp:%d C,Memavailable:%d KB";
	log_add(1,s,100*cpu_usage,loadavg,temp,Memavailable);
	log_print_all();
	return 0;
}*/