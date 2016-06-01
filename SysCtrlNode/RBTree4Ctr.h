#include "RBTree.h"
/************************************************************************/
//ID栈的定义
typedef struct rec_ID_stack{
    int ID;																	//回收的ID信息
    struct rec_ID_stack   *next;											//指针域
	int blank[8];															//保持地址对齐
}rec_ID_stack;

/************************************************************************/
//负载节点标识队列
typedef struct queue{
	int n ;
	struct queue *next;
}queue;


RBTree tree[6];																//树根节点
const long P_SIZE = 24;														//文件中每个节点的存储长度，20字节字符数组，4字节long
const char nul[30] = "****************************\n" ;                     //删除填充
int sum_id= 0;																//全局变量sum_id，表示已分配用户id总数
int exist_sum_id= 0;														//表示现存的id总数。
rec_ID_stack * top;															//栈顶指针
queue * QueTail, *QueHead;                                                  //定义队头队尾指针


//队列初始化
void initQue()
{
	QueHead = (queue *) MemoryPool_alloc();
	QueTail = QueHead;
	QueHead->n = QueTail->n = -1;
}

//入队
void inQueue(int serverNum)
{
     queue *q;
	 q = (queue*)MemoryPool_alloc();
	 q->n = serverNum;
	 if (QueHead == QueTail )												//如果是第一个元素入队
	 {
		 QueHead->next = q;													//队头的下一个节点指向q										
		 QueTail = q;														//队尾指向q
	 }																		
	 QueTail->next = q;														//队尾的下一个节点指向q
	 QueTail = q;															//队尾指向q
}

//出队
void outQueue(int *n)
{
	queue *q;
	q = QueHead->next;														//q指向队列的第一个元素
	*n = q->n;																//把服务器标识值取出
	if (q->next!=QueTail->next)												//是否到队尾
	{
		QueHead->next = q->next;											//没有到队尾
	}
	else
	{
		QueTail = QueHead;													//到队尾
	}
	Memory_recover(q);
	
}

/************************************************************************/
//进栈
void push( int id,rec_ID_stack *t)															//被删除用户的ID被回收入栈
{	   		  		       
    rec_ID_stack * r;
	r = (rec_ID_stack *) MemoryPool_alloc();
	r->ID=id;																				//分配id给新用户		
    r->next = t->next; 
    t->next = r;  

}

/************************************************************************/
//出栈
void pop(rec_ID_stack *t)																	//栈顶的用户信息出栈
{
	rec_ID_stack * p;
	p = t->next;
	t->next = t->next->next;	
	Memory_recover(p);
}


/********************************************************/
//n棵红黑树初始化,n小于等于5
void initRBS()
{
	int i;
	for (i = 0; i<6;i++)
	{
		initRB(&tree[i]);                       //红黑树的初始化
	}
}

void initIDStack()
{
	//栈顶节点的初始化
		top = (rec_ID_stack *) malloc(sizeof(rec_ID_stack));
		top->next = NULL;

}

/********************************************************/
//打开文件,成功返回1,失败返回0
FILE *openFile(const char *fName,FILE **fpp)
{
	while(1)
	{
		
	
		if( (*fpp = fopen(fName,"r+")) == NULL )
		{
			if( (*fpp = fopen(fName,"a+")) == NULL )
			{
				printf("创建文件失败,系统将重新创建...");
				continue;
			}
			else
			{
				fclose(*fpp);
			}
		
		}
		else
			break;
	}
	return *fpp;
}

/******************************************************/
//关闭文件
void closeFile(FILE *fpp)
{
	fclose(fpp);
}

/*******************************************************/
//写文件,成功返回1,失败返回0
int write2File(FILE *fpp,char *input,int id)
{
	clearerr(fpp);
	fseek(fpp,30L*id,0);
	//fputs成功返回0
	if(  !fputs(input,fpp) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************/
//读文件,读取完毕返回1,未完返回0,把文件内容中的用户名读进str
int readFromFile(FILE *fpp,char *str)
{
	char *p;
	p = str;
	if( fgets(str+1,30,fpp) )	
	{
		if (str[1] == '*')
		{
			push(sum_id,top);
			sum_id++;
		}
		//得到用户名
		while(p[0] != '*')
		{
			p++;
		}
		p[0] = '\0';

		if(str[1] != '\0')
		{	
			sum_id++;
			exist_sum_id++;
			
		}
		//未读取完
		return 0;
	}
	else
	{
		//完成整个文件的读取
		return 1;
	}

}

/*****************************************************/
//删除文件中某节点,成功返回1,失败返回0
int delFnode(FILE *fpp, int id)
{
	//节点定位
	fseek(fpp,30L*id,0);
	fputs(nul,fpp);
	return 1;
}

/*****************************************************/
//格式化字符串
char * formatStr(char *s)
{
	int i=0,n;
	n = strlen(s);
	for(i = n;i<28;i++)
	{
		s[i] = '*';
	}
	s[28] = '\n';
	s[29] = '\0';
	return s;
}

/*************************************************/
//给转发节点直接调用的函数，能够在内存中删除红黑树节点，并把该节点相关信息从文件中删除
int delNode(char *userN,char *fName,int k)
{
		PRBTNode p = NULL;
		FILE *fp;
		p = searchTNode(&tree[k],userN);
		//查找成功
		if( 0 == strcmp(p->Name,userN) )
		{	 
			push(p->ID,top);
			if(!openFile(fName,&fp))
			{//失败
				 return 0 ;
			}
			else
			{//成功
				delFnode(fp,p->ID);
				closeFile(fp);
			}
			 p = deleteRB(&tree[k],p);	
			 Memory_recover(p);
			 exist_sum_id--;
			 return 1;
		}	
		else
		{
			return 0;
		}
}

/***************************************/
//清空内存中的红黑树
int cleanRB()
{
	int i =1;
	PRBTNode p=NULL;
	while(tree[i].root!=tree[i].nil)				//若树头不是指向哨兵节点
	{
		p = deleteRB(&tree[i],tree[i].root);
		Memory_recover(p);
		i++;
	}
	return 1;
}

/********************************************/
//清除ID
int cleanStack()
{
	while(top->next != NULL)
	{
		pop(top);	
	}
	sum_id = 0;
	exist_sum_id = 0;
	return 1;

}

/*******************************************************/
//清除队列
int cleanQue()
{
	int n;
	while (QueHead != QueTail)
	{
		outQueue(&n);
	}
	return 1;
}
/********************************************************/
//所有初始化
void allinit()
{
	initRBS();
	initIDStack();
	initQue();
	MemoryPool_init(&Memory);
}


/*******************************************************/
//清空内存，ID栈，删除文件
void allClean(char *fName)
{
	cleanRB();
	cleanStack();
	cleanQue();
	remove(fName);
}


/*******************************************************/
//写入某棵红黑树内存,返回id,若ID为-1则需要动态分配ID,否则不需要
int write2RBTree(char *userN,int k,int ID)
{
	//构建红黑树  		       
	int id = 0;
	PRBTNode p = NULL;

	if(-1 == ID)
	{//动态分配
		if (top->next != NULL)
		{
			id=top->next->ID;
			pop(top);	
		}
		else{
			id = sum_id;													//分配id给新用户
			sum_id++;														//累加用户id个数 
		}

	}
	else
	{//直接取ID
		id = ID;
	}

	p = insertRB(&tree[k],userN,id);
	exist_sum_id++;

	return p->ID;

}


/*************************************************/
//给转发节点直接调用的函数，把节点备份到文件中,成功返回1，失败返回0
int backup2File(char *userN,char *fName,int id)
{
	char str1[30];
	char *s;
	FILE *fp;
	char temp[10];
	//写入文件
	//首先要格式化写入文件的字符串
	//1、30个字符，30个字节
	//2、格式按照："用户名，ID****..."后面用'*'填充满
	if(!openFile(fName,&fp))
	{//失败
		
		return 0 ;
	}
	else
	{//成功
		
		strcpy(str1,userN);
		s = formatStr(str1);
		fseek(fp,0L,0);
		write2File(fp,s,id);
		closeFile(fp);
		return 1;
	}
}

/*******************************************************/
//把临时树中的树头转存致树k
void changeTree(char *uName,int k,int ID)
{
	insertRB(&tree[k],uName,ID);
}


/******************************************************/
//用临时树
void tempTree(int n)
{
	tree[0].nil = tree[n].nil;
	tree[0].root = tree[n].root;
	tree[0].root->color = tree[n].root->color;
	tree[0].root->freeNext = tree[n].root->freeNext;
	tree[0].root->ID = tree[n].root->ID;
	tree[0].root->left = tree[n].root->left;
	strcpy(tree[0].root->Name,tree[n].root->Name);
	tree[0].root->parent = tree[n].root->parent;
	tree[0].root->right = tree[n].root->right;
}


/*****************************************************/
//写日志文件
int write2Log(char *Node,char *fName,int isConnect)
{
	FILE *fp;
	SYSTEMTIME time;
	TCHAR strTime[256];
	GetLocalTime(&time);
	wsprintf(strTime,"%04d-%02d-%02d %02d:%02d:%02d ",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);	
	strcat(strTime,Node);

	if( isConnect == 1 )
	{
		strcat(strTime,"启动\n");
	}
	else
	{
		strcat(strTime,"关闭\n");
	}


	if( (fp = fopen(fName,"a+")) == NULL )
	{
		printf("创建日志文件失败!");
	}
	else
	{
		fputs(strTime,fp); 
		fclose(fp);
	}
	return 0;
}