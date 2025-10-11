#include"logdir.h"
#define MAXLOGDIR 10   //日志链表的最大节点数
/*static struct{         //日志系统内部判断依据L
  int level;
  time_t timeL;
}L;*/

LogLinklist L_head = NULL;       //日志系统头指针
LogLinklist L_tail = NULL;


void log_init()
{
    L_head =(LogNode *)malloc(sizeof(LogNode));
	if(L_head==NULL) 
	{
		perror("L_head Init failed");
	}
	L_head->next=NULL;
    L_tail=L_head;
}


_Bool log_add(int level, const char *format, ...)   //有头结点
{
    LogNode *newNode=(LogNode *)malloc(sizeof(LogNode));
    if(newNode==NULL)
    {
	 perror("Create new Node failed");
	 return -1;
    }
	//初始化新节点
    if (time(&newNode->timestamp) == (time_t)-1) // 时间获取失败的处理
	{
    perror("time() failed");       // 可以设置一个默认时间或继续执行
    }
    newNode->level=level;
    newNode->next=NULL;
    printf("调试前%s\n",format);

	va_list args;                   //可变参数的解析,新节点信息获取
	va_start(args,format);
	vsnprintf(newNode->message,sizeof(newNode->message),format,args);
	va_end(args);
    
	printf("调试后:%s\n",newNode->message);
 
	newNode->next=L_head->next; 
	L_head->next=newNode;

    return 0;
}
//错误：没有处理空链表的情况，函数声明与实际返回类型不一致，缺少时间获取失败的处理

void log_print_all()
{
	int i=1;
    LogNode *p=L_head->next;
    while(p!=NULL&&i<=10)
    {
	 printf("日志结点(%d):%s\n",i,p->message);
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