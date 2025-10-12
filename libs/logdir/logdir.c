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
	log_fp=fopen("/home/devuser/Desktop/code/System_status_monitor/libs/log","a+");
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
		fprintf(log_fp,"[%s] %s %s\n",s,level_strings[level],newNode->message);
        fflush(log_fp);  // 立即刷新，确保数据写入磁盘
	}

}
//错误：没有处理空链表的情况，函数声明与实际返回类型不一致，缺少时间获取失败的处理

void log_print_recent(int i)
{
	FILE *fp=fopen("/home/devuser/Desktop/code/System_status_monitor/libs/log","r");
	if(fp==NULL) perror("Cannot open log");
	fseek(fp,-1,SEEK_END);    //fseek里,当whence是SEEK_END时，若偏移量是0，则是在最后一个字符之后，读取的话是EOF;
	//putchar(z);
	long pos=ftell(fp)-1;
    char buffer[256];
	int num=0;  
	char current_char;
	while(pos>=0&&num<i)
	{
		fseek(fp,-1,SEEK_CUR);
		current_char=fgetc(fp);     //fp赋值，然后文件指针自动往靠近文件末尾的方向移动一个字节
		if(current_char=='\n')
		{ 
			long before_read=ftell(fp);
            fgets(buffer, sizeof(buffer), fp);
			printf("%s",buffer);
            num++;
			fseek(fp,before_read,SEEK_SET);

		}
		pos--;
		fseek(fp,-1,SEEK_CUR);

	}
    fclose(fp);
}
long int get_file_size(char *file_path)  // 获取文件大小
{ 
    struct stat file_stat;
	long int file_size;
    if(stat(file_path,&file_stat)==0)
	{
         file_size=file_stat.st_size;
	}
	else file_size=-1;
	return file_path;
}
void log_rollover()
{
	const int MAX_BACKUP_FILES=4;   //最大备份数量，加上原文件就5个
	char old_path[128];
    snprintf(old_path,sizeof(old_path),"/home/devuser/Desktop/code/System_status_monitor/libs/log.%d",MAX_BACKUP_FILES);
	remove(old_path);  //每次都尝试删除最旧的文件

	for(int i=MAX_BACKUP_FILES-1;i>0;i--)
	{
       char old_name[128],new_name[128];
	   snprintf(old_name,sizeof(old_name),"/home/devuser/Desktop/code/System_status_monitor/libs/log.%d",i);
	   snprintf(new_name,sizeof(old_name),"/home/devuser/Desktop/code/System_status_monitor/libs/log.%d",i+1);
       rename(old_name,new_name);
	}
    rename("/home/devuser/Desktop/code/System_status_monitor/libs/log","/home/devuser/Desktop/code/System_status_monitor/libs/log.1");  
	//当前日志文件改为第一备份
    

}
