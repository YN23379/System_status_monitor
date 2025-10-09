
#include"logdir.h"


typedef struct Log_entry {  //定义日志节点，时间，级别（I，日常信息，W，警告信息，E，错误信息，显示级别越来越高）
	time_t timestamp;        
	char level;            //信息等级
	char data[256];         //信息，和指针域
	struct Log_entry* next;
}LogNode,*LogLinklist;

LogLinklist L_head = NULL;
LogLinklist L_tail = NULL;

_Bool log_init(void)
{
    L_head = NULL;
    L_tail = NULL;
    printf("Log system initialized\n");
    return 0;
}

_Bool log_add(char level, const char *message,...)  //可变参数，高级货！！！   没思路，高级个蛋,不用了   好吧不用不行，但现在有些理解了
{
    if(L_head==NULL)                          //健壮性！
    {
        perror("L_head is NULL");
        return -1;
    }
	LogNode *newNode;
	newNode=(LogNode *)malloc(sizeof(LogNode));
    if(newNode==NULL)
    {
        perror("Cannot malloc to newNode");
        return -1;
    }
    
    newNode->timestamp = time(NULL);      // 初始化新节点
    newNode->level = level;
    newNode->next = NULL;                 //防污染
    strncpy(new_node->message, message, 255);
    new_node->message[255] = '\0';       // 确保字符串结束
    
    va_list args;
    va_start(args,message);
    vsnprintf()

} 