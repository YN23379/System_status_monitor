#include"logdir.h"
#define MAXLOGDIR 10   //日志链表的最大节点数
/*static struct{         //日志系统内部判断依据L
  int level;
  time_t timeL;
}L;*/

//LogLinklist L_tail = NULL;
static FILE *log_fp=NULL;            // 日志输出文件存放位置
static const char* level_strings[] = {   //level等级
  "INFO", "WARN", "FATAL"
};
void log_init()                   //初始化日志系统
{
	log_fp=fopen("/home/devuser/Desktop/code/System_status_monitor/libs/log","a+");
	if(log_fp==NULL)
	{
		perror("Fopen Log file failed");
		fclose(log_fp);
	}
}

void log_add(int level, const char *format, ...)   
{
	log_init();
    if(level<0||level>2)
	{
		level=0;
	}
    char time_buf[64];
	time_t now=time(NULL);
	struct tm *tm_info=localtime(&now);
	strftime(time_buf,sizeof(time_buf),"%Y-%m-%d %H:%M:%S", tm_info);
	
    char message[256];
	va_list args;                   //可变参数的解析,新节点信息获取
	va_start(args,format);
	vsnprintf(message,sizeof(message),format,args);
	va_end(args);
  

	if(log_fp!=NULL)   //日志系统文件输出
	{
		fprintf(log_fp,"[%s] %s %s\n",time_buf,level_strings[level],message);
        fflush(log_fp);  // 立即刷新，确保数据写入磁盘
	}
	fclose(log_fp);
}

void log_print_recent(int i)
{
	log_init();
	fseek(log_fp,0,SEEK_END);    //fseek里,当whence是SEEK_END时，若偏移量是0，则是在最后一个字符之后，读取的话是EOF;
	//putchar(z);
	long pos=ftell(log_fp);
	if (pos == 0) {
        printf("Log file is empty\n");
        fclose(log_fp);
        return;
    }
	fseek(log_fp,-1,SEEK_CUR);
    char buffer[256];
	int num=0;  
	char current_char;
	while(pos>0&&num<i)   //不能让位置标识符到首字符之后还往前读
	{
		fseek(log_fp,-1,SEEK_CUR);
		if(ftell(log_fp)==0)
		{
			fgets(buffer, sizeof(buffer), log_fp);
			printf("%s",buffer);
			fclose(log_fp);
			return;
		}
		current_char=fgetc(log_fp);     //fp赋值，然后文件指针自动往靠近文件末尾的方向移动一个字节
		if(current_char=='\n'&&ftell(log_fp)!=0)
		{ 
			long before_read=ftell(log_fp);
            fgets(buffer, sizeof(buffer), log_fp);
			printf("%s",buffer);
            num++;
			fseek(log_fp,before_read,SEEK_SET);

		}
		pos--;
		fseek(log_fp,-1,SEEK_CUR);

	}
    fclose(log_fp);
}

long int get_file_size(char *file_path)  // 获取文件大小
{ 
    struct stat file_stat;
	long int file_size;
	if (stat(file_path, &file_stat) == -1) 
	{
    printf("stat失败: %s\n", strerror(errno));
    }
    if(stat(file_path,&file_stat)==0)
	{
         file_size=file_stat.st_size;
	}
	else file_size=-1;
	return file_size;
}

void log_rollover()
{
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
